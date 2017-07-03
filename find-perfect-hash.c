/* Copyright (C) 2017 Douglas Bagnall <douglas@halo.gen.nz> GPL2+ */

#include "find-perfect-hash-helpers.h"
#include "find-perfect-hash.h"
#include <locale.h>
#include "argparse/argparse.h"


#define FNV64 1099511628211UL
#define FNV64_SEED 14695981039346656037UL
#define FNV32 16777619
#define FNV32_SEED 2166136261u
#define DJB_SEED 5381


enum {
	HASH_FNV32 = 0,
	HASH_FNV64,
	HASH_DJB,

	HASH_LAST
};

const char * const hash_names[] = {
	[HASH_FNV32] = "fnv32",
	[HASH_FNV64] = "fnv64",
	[HASH_DJB] = "djb"
};

static void init_hash(struct hashdata *hash, const char *string,
		      bool case_insensitive, uint hash_id)
{
	uint32_t djb = DJB_SEED;
	uint32_t fnv32 = FNV32_SEED;
	uint64_t fnv64 = FNV64_SEED;
	const char *s;
	for (s = string; *s != '\0'; s++) {
		uint8_t c = (uint8_t)*s;
		if (case_insensitive) {
			c &= 0x5f;
		}
		fnv32 ^= c;
		fnv32 *= FNV32;
		fnv64 ^= c;
		fnv64 *= FNV64;
		djb = ((djb << 5) + djb) ^ c;
	}
	hash->string = string;
	hash->stringlen = s - string;
	hash->running_hash = 0;
	if  (hash_id == HASH_DJB) {
		hash->raw_hash = djb;
	} else if (hash_id == HASH_FNV64) {
		hash->raw_hash = fnv64;
	} else {
		hash->raw_hash = fnv32;
	}
}

static struct hashdata *new_hashdata(struct strings *strings,
				     bool case_insensitive,
				     uint hash_id)
{
	int i;
	struct hashdata *data = calloc(strings->n_strings,
				       sizeof(data[0]));

	for (i = 0; i < strings->n_strings; i++) {
		init_hash(&data[i], strings->strings[i],
			  case_insensitive, hash_id);
	}
	return data;
}


static inline bool find_mask_collision(struct hashcontext *ctx,
				       uint64_t *values,
				       uint n_values,
				       uint64_t hash_mask)
{
	uint i;
	uint index;
	uint bucket_mask = n_values - 1;
	bool collides = false;

	for (i = 0; i < ctx->n; i++) {
		uint64_t h = ctx->data[i].raw_hash;
		h &= hash_mask;
		if (unlikely(h == 0ULL)) {
			collides = true;
			goto end;
		}

		/* hash of hash */
		uint64_t h2 = (h >> 37) ^ (h >> 29) ^ (h >> 16) ^ h;

		index = h2 & bucket_mask;
		while (values[index] != 0) {
			if (values[index] == h) {
				collides = true;
				goto end;
			}
			index = (index + 1) & bucket_mask;
		}
		values[index] = h;
	}
  end:
	memset(values, 0, n_values * sizeof(uint64_t));
	return collides;
}


static uint64_t find_hash_bit_filter(struct hashcontext *ctx,
				     struct hashdata *data)
{
	uint hash_size;
	uint64_t i, rounds = 1000000;
	uint64_t best_mask, mask, mask_mask;
	uint64_t mul = 2862933555777941757UL;

	if (ctx->hash_id == HASH_FNV64) {
		/* unlikely to work! */
		hash_size = 64;
		best_mask = UINT64_MAX;
		mask = 0x5555aaaa5555aaaa;
		mask_mask = UINT64_MAX;
	} else {
		hash_size = 32;
		best_mask = UINT32_MAX;
		mask = 0x5555aaaa;
		mask_mask = UINT32_MAX;
	}

	uint n_buckets = 256;
	while (n_buckets < ctx->n * 2) {
		n_buckets *= 2;
	}
	printf("using %u buckets\n", n_buckets);
	uint64_t *buckets = calloc(n_buckets, sizeof(uint64_t));

	uint best_size = hash_size;

	printf("searching for a minimal mask in %lu rounds\n", rounds);
	uint32_t n_tests = 0;
	uint32_t n_too_small = 0;

	for (i = 0; i < rounds; i++) {
		mask *= mul;
		mask &= mask_mask;
		uint bits = __builtin_popcountll(mask);
		if (bits >= best_size) {
			continue;
		}
		if (bits < MIN(ctx->bits + 2, best_size - 1)) {
			n_too_small++;
			continue;
		}
		n_tests++;
		bool collides = find_mask_collision(ctx,
						    buckets,
						    n_buckets,
						    mask);
		if (! collides) {
			printf("found %u bit mask %lx after %'lu rounds\n",
			       bits, mask, i);
			best_mask = mask;
			best_size = bits;
		}
	}
	free(buckets);
	printf("found mask %lx of %u bits\n", best_mask, best_size);
	printf("%lu too big, %u tests, %u too small\n",
	       i - n_tests - n_too_small, n_tests, n_too_small);
	return best_mask;
}


static bool check_raw_hash(struct hashcontext *ctx)
{
	uint i;
	uint mask;
	uint buckets = 256;
	uint64_t *values;
	uint index;
	while (buckets < ctx->n * 2) {
		buckets *= 2;
	}
	mask = buckets - 1;

	values = calloc(buckets, sizeof(uint64_t));

	for (i = 0; i < ctx->n; i++) {
		uint64_t h = ctx->data[i].raw_hash;
		if (h == 0ULL) {
			printf("'%s' hashes to zero, with which we can't cope\n",
			       ctx->data[i].string);
			goto bad;
		}
		index = h & mask;
		while (values[index] != 0) {
			if (values[index] == h) {
				goto bad;
			}
			index = (index + 1) & mask;
		}
		values[index] = h;
	}
	free(values);
	return true;
  bad:
	free(values);
	return false;
}


static void save_db(struct hashcontext *ctx)
{
	if (ctx->db_name != NULL &&
	    ctx->n_good_params != 0 &&
	    ctx->good_params != NULL) {
		char *backup;
		int ret = asprintf(&backup, "%s.bak", ctx->db_name);
		if (ret < 5) {
			printf(COLOUR(C_RED, "could not save backup "
				      "param DB %s.bak\n"),
			       ctx->db_name);
			if (ret >= 0) {
				free(backup);
			}
			return;
		}
		rename(ctx->db_name, backup);
		free(backup);
		FILE *f = fopen(ctx->db_name, "w");
		size_t bytes = fwrite(ctx->good_params, sizeof(uint64_t),
				      ctx->n_good_params, f);
		fclose(f);
		if (bytes != ctx->n_good_params) {
			printf(COLOUR(C_RED, "could not write db file"
				      "(wrote %zu expected %lu)\n"),
			       bytes, ctx->n_good_params * sizeof(uint64_t));
		}
	}
}

static void add_db_param(struct hashcontext *ctx, uint64_t param)
{
	if (ctx->good_params == NULL) {
		return;
	}
	///XXX perhaps sort or something
	for (uint i = 0; i < ctx->n_good_params; i++) {
		if (ctx->good_params[i] == param) {
			printf("param #%d %lx (%lx↻%lu) already saved\n", i,
			       param, MR_MUL(param), MR_ROT(param));
			return;
		}
	}
	if ((ctx->n_good_params & 15) == 0) {
		printf("reallocing param DB of size %u\n", ctx->n_good_params);
		ctx->good_params = realloc(ctx->good_params,
					   (ctx->n_good_params + 16) * sizeof(uint64_t));
		if (ctx->good_params == NULL) {
			printf(COLOUR(C_RED, "failed to realloc param DB\n"));
			return;
		}
	}
	ctx->good_params[ctx->n_good_params] = param;
	ctx->n_good_params++;
	printf("saved param  #%u %lx (%lx↻%lu)\n", ctx->n_good_params,
	       param, MR_MUL(param), MR_ROT(param));
	save_db(ctx);
}


static void read_db(struct hashcontext *ctx, const char *db_name)
{
	ctx->n_good_params = 0;
	ctx->good_params = NULL;
	if (db_name != NULL) {
		int len;
		size_t len2;
		FILE *f = fopen(db_name, "r");
		if (f == NULL) {
			printf(COLOUR(C_CYAN, "could not open param DB %s\n"),
			       db_name);
			ctx->n_good_params = 0;
			ctx->good_params = malloc(16 * sizeof(uint64_t));
			return;
		}
		fseek(f, 0, SEEK_END);
		len = ftell(f);
		rewind(f);
		ctx->good_params = malloc(len + 16 * sizeof(uint64_t));
		if (ctx->good_params == NULL) {
			printf(COLOUR(C_RED, "could not alloc %d bytes\n"),
			       len);
			fclose(f);
			return;
		}
		len2 = fread(ctx->good_params, 1, len, f);
		fclose(f);
		if (len != (int)len2) {
			printf(COLOUR(C_RED, "error reading %d bytes"
				      "(got %zu)\n"),
			       len, len2);
			return;
		}
		ctx->n_good_params = len2 / sizeof(uint64_t);
	}
}

static void import_text_parameters(struct hashcontext *ctx, const char *filename)
{
	/* fits exactly one case at the moment */
	int matched;
	uint64_t mul, rot, param;
	FILE *f = fopen(filename, "r");
	while (true) {
		matched = fscanf(f, " ×%lx ↻%lu & %*x", &mul, &rot);
		if (matched != 2) {
			printf("match %d, stopping\n", matched);
			break;
		}
		param = MUL_ROT_TO_PARAM(mul, rot);
		printf("found %lx\n", param);
		add_db_param(ctx, param);
	}
	fclose(f);
}


static inline uint32_t hash_component(uint64_t *params, uint i, uint64_t x)
{
	uint64_t param = params[i];
	uint64_t rot = MR_ROT(param);
	uint64_t mul = MR_MUL(param);
	uint32_t mask = MR_MASK(i);
	return MR_COMPONENT(x, mul, rot, mask);
}

static inline uint32_t unmasked_hash(uint64_t raw_hash,
				     uint64_t *params, uint n)
{
	uint i;
	uint32_t hash = 0;
	for (i = 0; i < n; i++) {
		uint32_t comp = hash_component(params, i, raw_hash);
		hash ^= comp;
	}
	return hash;
}

static inline uint32_t running_unmasked_hash(uint64_t raw_hash,
					     uint32_t running_hash,
					     uint64_t *params, uint n)
{
	uint32_t comp = hash_component(params, n - 1, raw_hash);
	return running_hash ^ comp;
}


static inline void update_running_hash(struct hashcontext *ctx,
				       uint64_t *params, uint n)
{
	uint i;
	for (i = 0; i < ctx->n; i++) {
		ctx->data[i].running_hash = unmasked_hash(
			ctx->data[i].raw_hash,
			params, n);
	}
}

static bool reorder_params(struct multi_rot *c, uint a, uint b)
{
	if (a == b) {
		return true;
	}
	uint64_t *params = c->params;
	uint64_t p1 = params[a];
	uint64_t p2 = params[b];
	uint64_t mul1 = p1 & ~MR_ROT_MASK;
	uint64_t mul2 = p2 & ~MR_ROT_MASK;
	uint64_t r1 = MR_ROT(p1);
	uint64_t r2 = MR_ROT(p2);
	uint64_t n1 = (r1 - a + b) & 63;
	uint64_t n2 = (r2 - b + a) & 63;
	uint64_t new1 = n1 * MR_ROT_STEP;
	uint64_t new2 = n2 * MR_ROT_STEP;

	params[b] = mul1 | new1;
	params[a] = mul2 | new2;
	return true;
}

static uint64_t remove_unused_param_bits(struct hashcontext *ctx,
					 uint64_t param, uint n)
{
	uint64_t mul = MR_MUL(param);
	uint64_t rot = MR_ROT(param);
	uint i, j;
	uint64_t original = param;
	uint32_t mask = MR_MASK(n);
	for (i = 59; i > 1; i--) {
		uint64_t mul2 = mul & ((1ULL << i) - 1ULL);
		for (j = 0; j < ctx->n; j++) {
			uint64_t x = ctx->data[j].raw_hash;
			if (MR_COMPONENT(x, mul, rot, mask) !=
			    MR_COMPONENT(x, mul2, rot, mask)) {
				goto low_bits;
			}
		}
		param = MUL_ROT_TO_PARAM(mul2, rot);
	}
  low_bits:
	for (i = 0; i < 50; i++) {
		uint64_t mul2 = mul & ((1ULL << i) - 1ULL);
		for (j = 0; j < ctx->n; j++) {
			uint64_t x = ctx->data[j].raw_hash;
			if (MR_COMPONENT(x, mul, rot, mask) !=
			    MR_COMPONENT(x, mul2, rot, mask)) {
				goto exit;
			}
		}
		printf("param %u can drop low bit %u\n", n, i);
		param = MUL_ROT_TO_PARAM(mul2, rot);
	}
  exit:
	printf("param %u: %lx -> %lx\n", n, original, param);
	return param;
}

static void remove_unused_bits(struct hashcontext *ctx,
			       struct multi_rot *c)
{
	uint i;
	for (i = 0; i < ctx->n_params; i++) {
		c->params[i] = remove_unused_param_bits(ctx,
							c->params[i], i);
	}
}

static uint test_params(struct hashcontext *ctx,
			uint64_t *params, uint n)
{
	int j;
	uint collisions = 0;
	uint64_t *hits = (uint64_t *)ctx->hits;
	for (j = 0; j < ctx->n; j++) {
		uint32_t hash = unmasked_hash(ctx->data[j].raw_hash,
					      params, n);
		uint32_t f = hash >> (uint64_t)6;
		uint64_t g = 1UL << (hash & (uint64_t)63);
		collisions += (hits[f] & g) ? 1 : 0;
		hits[f] |= g;
	}
	memset(hits, 0, (1 << ctx->bits) / 8);
	return collisions;
}

static uint test_params_l2(struct hashcontext *ctx,
			   uint64_t *params, uint n)
{
	int j;
	uint16_t *hits = ctx->hits;
	uint64_t c2 = 0;
	uint n_bits = n + BASE_N - 1;
	uint16_t worst = 0;
	for (j = 0; j < ctx->n; j++) {
		uint32_t hash = unmasked_hash(ctx->data[j].raw_hash,
					      params, n);
		uint16_t h = hits[hash];
		c2 += h;
		h++;
		worst = MAX(h, worst);
		hits[hash] = h;
	}
	uint64_t w = worst;
	uint64_t score = c2 + w * w * w;

	memset(hits, 0, (1 << n_bits) * sizeof(hits[0]));
	return score;
}


static uint find_worst_param(struct hashcontext *ctx,
			     struct multi_rot *c,
			     uint n_params,
			     uint *mean_score)
{
	uint i, score;
	uint worst_param = 1;
	uint sum = 0;
	uint best_score = UINT_MAX;
	uint full_score = test_params_l2(ctx, c->params, n_params);
	printf("with %u params: %u score\n", n_params,
	       full_score);

	for (i = 1; i < n_params; i++) {
		reorder_params(c, i, n_params - 1);
		score = test_params_l2(ctx, c->params, n_params - 1);
		sum += score;
		printf("without %2u: %4u score gain %u\n", i,
		       score, score - full_score);
		if (score < best_score) {
			best_score = score;
			worst_param = i;
		}
		reorder_params(c, i, n_params - 1);
	}
	*mean_score = (sum + (n_params - 1) / 2) / (n_params - 1);
	printf("mean score: %4u\n", *mean_score);
	printf("worst is %2u: %4u score, gain %u\n", worst_param,
	       best_score,  best_score - full_score);
	return worst_param;
}


static uint describe_hash(struct hashcontext *ctx,
			  struct multi_rot *c,
			  struct multi_rot *c2,
			  uint n_params,
			  bool check_collisions)
{
	int i, j;

	printf("bits %u mask %x\n", ctx->bits,
	       (1 << ctx->bits) - 1);

	uint collisions = test_params(ctx, c->params, n_params);

	if (collisions != c->collisions && check_collisions) {
		printf("\033[01;31m collisions mismatch! \033[00m\n"
		       "presumed %u actual %u\n", c->collisions, collisions);
	}
	if (collisions == 0) {
		printf("\033[01;31m");
	};
	printf("collisions %-3u ", collisions);
	printf("\033[00mbits: ");
	for (i = 0; i < n_params; i++) {
		uint64_t x = c->params[i];
		uint64_t mul = MR_MUL(x);
		uint mask = MR_MASK(i);
		if (i) {
			printf("\033[01;34m ^ ");
		}
		bool duplicate = false;
		if (c2 != NULL) {
			for (j = 1; j < n_params; j++) {
				if (mul == MR_MUL(c2->params[j])) {
					duplicate = true;
					break;
				}
			}
		}
		if (duplicate) {
			printf("\033[01;33m");
		} else {
			printf("\033[01;%dm", 36 + (i != 0));
		}
		printf("×%015lx ↻%-2lu & %04x ",
		       mul, MR_ROT(x), mask);
	}
	printf("\033[00m\n");
	return collisions;
}

static uint test_params_running(struct hashcontext *ctx,
				uint64_t *params, uint n,
				uint best_collisions)
{
	int j;
	uint collisions = 0;
	uint64_t *hits = (uint64_t *)ctx->hits;
	uint32_t hash;
	uint64_t param = params[n - 1];
	uint64_t rot = MR_ROT(param);
	uint64_t mul = MR_MUL(param);
	uint32_t mask = MR_MASK(n - 1);

	for (j = 0; j < ctx->n; j++) {
		uint32_t comp = MR_COMPONENT(ctx->data[j].raw_hash,
					     mul, rot, mask);

		hash = (ctx->data[j].running_hash ^ comp);
		uint32_t f = hash >> 6;
		uint64_t g = 1UL << (hash & 63);
		collisions += (hits[f] & g) ? 1 : 0;
		hits[f] |= g;
		if (collisions >= best_collisions) {
			break;
		}
	}
	memset(hits, 0, (1 << ctx->bits) / 8);
	return collisions;
}

static uint64_t test_params_with_l2_running(struct hashcontext *ctx,
					    uint64_t *params, uint n)
{
	int j;
	uint16_t *hits = ctx->hits;
	uint64_t c2 = 0;
	uint64_t param = params[n - 1];
	uint64_t rot = MR_ROT(param);
	uint64_t mul = MR_MUL(param);
	uint32_t mask = MR_MASK(n - 1);
	uint16_t worst = 0;
	uint n_bits = n + BASE_N - 1;

	for (j = 0; j < ctx->n; j++) {
		uint32_t comp = MR_COMPONENT(ctx->data[j].raw_hash,
					     mul, rot, mask);
		uint32_t hash = ctx->data[j].running_hash ^ comp;
		uint16_t h = hits[hash];
		c2 += h;
		h++;
		worst = MAX(h, worst);
		hits[hash] = h;

	}
	uint64_t w = worst;
	uint64_t score = c2 + w * w * w;

	memset(hits, 0, (1 << n_bits) * sizeof(hits[0]));

	return score;
}


static uint64_t calc_best_error(struct hashcontext *ctx, uint n_params)
{
	uint n_bits = BASE_N + n_params;
	uint n = 1 << n_bits;
	uint q = ctx->n / n;
	uint r = ctx->n % n;
	uint min_h = q + (r ? 1 : 0);
	uint mh = 1 << (ctx->bits - n_params - BASE_N);
	printf("worst scores: max allowable %u; best conceivable %u\n",
	       mh, min_h);
	uint64_t sum = q * (n * (q - 1) + 2 * r) / 2;
	return sum + min_h * min_h * min_h;
}

static void reset_close_params(struct hashcontext *ctx)
{
	size_t cp_size = N_CLOSE_PARAMS * sizeof(struct close_param);
	/*0xff so all scores are UINT_MAX*/
	memset(ctx->close_params, 0xff, cp_size);
}

static inline void add_close_param(struct hashcontext *ctx,
				   uint64_t param, uint score)
{
	struct close_param *heap = ctx->close_params;
	uint child;
	if (score >= heap[0].score) {
		return;
	}
	struct close_param undo = heap[0];
	uint pos = 0;

	for (child = 1;
	     child < N_CLOSE_PARAMS - 2;
	     child = 2 * pos + 1) {
		child += (heap[child].score < heap[child + 1].score);
		if (score >= heap[child].score) {
			break;
		}
		heap[pos] = heap[child];
		pos = child;
	}

	if (score == heap[child].score &&
	    param == heap[child].param) {
		/* This one is already there (at least once) and we want to
		   avoid filling it up monotonously. So we unwind what we did,
		   which is simple because we know we shifted everything to
		   its parent.

		   (of course there could be other duplicates down other
		   branches) */
		do {
			uint parent = (child - 1) / 2;
			heap[child] = heap[parent];
			child = parent;
		} while (child);
		heap[0] = undo;
		return;
	}

	heap[pos].score = score;
	heap[pos].param = param;
}


enum next_param_tricks {
	TRICK_NONE = 0,
	TRICK_GOOD_PARAM,

	TRICK_BEST_PARAM_BIT_FLIP,
	TRICK_CLOSE_PARAM_BIT_FLIP,

	TRICK_BEST_PARAM_ROTATE,
	TRICK_CLOSE_PARAM_ROTATE,

	TRICK_BEST_PARAM_SHIFT,
	TRICK_CLOSE_PARAM_SHIFT,

	TRICK_BEST_TRIPLE_BIT_FLIP,
	TRICK_CLOSE_TRIPLE_BIT_FLIP,

	TRICK_BEST_QUAD_BIT_FLIP,
	TRICK_CLOSE_QUAD_BIT_FLIP,
};

#define t_random(x) x
#define t_db(x) C_CYAN x C_NORMAL
#define t_best(x) C_YELLOW x C_NORMAL
#define t_close(x) C_LT_GREEN x C_NORMAL

const char * const trick_names[] = {
	[TRICK_NONE] = t_random("random"),
	[TRICK_GOOD_PARAM] = t_db("good param"),

	[TRICK_BEST_PARAM_BIT_FLIP] = t_best("best param bit flip"),
	[TRICK_CLOSE_PARAM_BIT_FLIP] = t_close("close param bit flip"),

	[TRICK_BEST_PARAM_ROTATE] = t_best("best param rotate"),
	[TRICK_CLOSE_PARAM_ROTATE] = t_close("close param rotate"),

	[TRICK_BEST_PARAM_SHIFT] = t_best("best param shift"),
	[TRICK_CLOSE_PARAM_SHIFT] = t_close("close param shift"),

	[TRICK_BEST_TRIPLE_BIT_FLIP] = t_best("best triple flip"),
	[TRICK_CLOSE_TRIPLE_BIT_FLIP] = t_close("close triple flip"),

	[TRICK_BEST_QUAD_BIT_FLIP] = t_best("best quad flip"),
	[TRICK_CLOSE_QUAD_BIT_FLIP] = t_close("close quad flip"),
};
#undef t_random
#undef t_db
#undef t_best
#undef t_close

static inline uint64_t next_param(struct hashcontext *ctx,
				  uint64_t round, uint n_params,
				  uint64_t best, uint64_t close,
				  uint close_score,
				  uint *used_trick)
{
	/* these ones work perfect for the first 2 params in
	   ldap_display_names */
	uint64_t p, rot;
	const uint64_t best_param_chance = best ? BEST_PARAM_CHANCE : 0;
	struct rng *rng = ctx->rng;

	add_close_param(ctx, close, close_score);

	if (round < ctx->n_good_params) {
		*used_trick = TRICK_GOOD_PARAM;
		return ctx->good_params[round];
	}
	uint64_t c = best;
	uint threshold = N_CLOSE_PARAMS + best_param_chance;
	uint i = rand64(rng) & CLOSE_PARAM_RANGE;
	if (i < threshold) {
		p = rand64(rng);
		uint trick = p & 3;
		p >>= 2;
		uint64_t a;
		if (i < N_CLOSE_PARAMS) {
			c = ctx->close_params[i].param;
		}
		switch (trick) {
		case 0:
			if (c == best) {
				*used_trick = TRICK_BEST_PARAM_BIT_FLIP;
			} else {
				*used_trick = TRICK_CLOSE_PARAM_BIT_FLIP;
			}
			/* Always change at least one multiplier. */
			do {
				a = 1ULL << (p & 63ULL);
				p >>= 6;
			} while (p != 0 && MR_MUL(a) == 0);
			a |= 1ULL << (p & 63ULL);
			return c ^ a;
		case 1:
			if (c == best) {
				*used_trick = TRICK_BEST_PARAM_ROTATE;
			} else {
				*used_trick = TRICK_CLOSE_PARAM_ROTATE;
			}
			p = rand64(rng) & 63;
			return ROTATE(c, p & 63);
		case 2:
		{
			uint64_t mul = MR_MUL(c);
			uint64_t rot = MR_ROT(c);
			uint64_t shift = p & 7;
			if (c == best) {
				*used_trick = TRICK_BEST_PARAM_SHIFT;
			} else {
				*used_trick = TRICK_CLOSE_PARAM_SHIFT;
			}
			if (p & 8) {
				mul <<= shift;
				rot += shift;
			} else {
				mul >>= shift;
				rot -= shift;
			}
			rot &= 63;
			return MUL_ROT_TO_PARAM(mul, rot);
		}
		case 3:
		{
			a = 1ULL << (p & 63ULL);
			p >>= 6;
			a |= 1ULL << (p & 63ULL);
			p >>= 6;
			a |= 1ULL << (p & 63ULL);
			bool quad = p & 1;
			if (quad) {
				p >>= 7;
				a |= 1ULL << (p & 63ULL);
			}
			if (c == best) {
				if (quad) {
					*used_trick = TRICK_BEST_QUAD_BIT_FLIP;
				} else {
					*used_trick = TRICK_BEST_TRIPLE_BIT_FLIP;
				}
			} else {
				if (quad) {
					*used_trick = TRICK_CLOSE_QUAD_BIT_FLIP;

				} else {
					*used_trick = TRICK_CLOSE_TRIPLE_BIT_FLIP;
				}
			}
			return c ^ a;
		}
		}
	}
	*used_trick = TRICK_NONE;
	uint64_t n_bits = n_params + BASE_N - 1;
	/* Some rotates are a bit useless. In particular we want to avoid the
	   lowest bit landing on the target bit. This of course only matters
	   with rounds that use rotate.

	   ROTATE shifts left, bringing low bits toward our bit, then suddenly
	   past.
	*/
	for (i = 9; i > 4; i--) {
		p = rand64(rng);
		rot = MR_ROT(p);
		if (n_bits - rot > i) {
			break;
		}
	}
	return p;
}

static inline bool test_pair(uint64_t param,
			     uint64_t raw_a,
			     uint64_t raw_b,
			     uint n)
{
	uint64_t rot = MR_ROT(param);
	uint64_t mul = MR_MUL(param);
	uint32_t mask = MR_MASK(n - 1);

	uint32_t a = MR_COMPONENT(raw_a,
				  mul, rot, mask);
	uint32_t b = MR_COMPONENT(raw_b,
				  mul, rot, mask);
	return a != b;
}


static uint test_all_pairs(uint64_t param,
			   struct tuple_list pairs,
			   uint n_params)
{
	uint i;
	uint collisions = 0;
	for (i = 0; i < pairs.n; i++) {
		collisions += ! test_pair(param,
					  pairs.raw[i * 2], pairs.raw[i * 2 + 1],
					  n_params);
	}
	return collisions;
}

static inline uint64_t test_pair_all_rot(uint64_t param,
					 uint64_t raw_a,
					 uint64_t raw_b,
					 uint n)
{
	uint64_t mul = MR_MUL(param);
	uint64_t all_a = raw_a * mul;
	uint64_t all_b = raw_b * mul;
	return all_a ^ all_b;
}

static uint test_all_pairs_all_rot(uint64_t *param,
				   struct tuple_list pairs,
				   uint n_params,
				   uint best_collisions,
				   uint64_t mask)
{
	uint i, j;
	uint n_bits = n_params + BASE_N - 1;
	uint64_t p = *param & ~MR_ROT_MASK;
	uint64_t mul = MR_MUL(p);
	uint8_t ones8[64] __attribute__((__aligned__(ALIGNMENT)));
	uint64_t *ones = (uint64_t*)ones8;

	ones[0] = ones[1] = ones[2] = ones[3] = 0ULL;
	ones[4] = ones[5] = ones[6] = ones[7] = 0ULL;

	for (i = 0; i < pairs.n; i++) {
		uint64_t a = pairs.raw[i * 2] * mul;
		uint64_t b = pairs.raw[i * 2 + 1] * mul;
		uint64_t c = a ^ b;
		uint64_t d;
		for (j = 0; j < 8; j++) {
			d = c >> (j * 8);
			d &= 0xffULL;
			d |= d << 28ULL;
			d &= 0x0000000f0000000fULL;
			d |= d << 14ULL;
			d &= 0x0003000300030003ULL;
			d |= d << 7ULL;
			d &= 0x0101010101010101ULL;
			ones[j] += d;
		}
	}
	uint8_t best_count = pairs.n - MIN(best_collisions, pairs.n);
	uint8_t best_i = 255;

	if (mask == 0 || mask == ~0ULL) {
		for (i = 0; i < 64; i++) {
			if (ones8[i] > best_count) {
				best_count = ones8[i];
				best_i = i;
			}
		}
	} else {
		for (i = 0; i < 64; i++) {
			if (mask & 1) {
				if (ones8[i] > best_count) {
					best_count = ones8[i];
					best_i = i;
				}
			}
			mask >>= 1;
		}
	}

	if (best_i < 64) {
		uint64_t rotate = n_bits - best_i - 1;
		rotate &= 63;
		*param = p + rotate * MR_ROT_STEP;
	}
	return pairs.n - best_count;
}


struct tuple_stats {
	uint min;
	uint max;
	double mean;
	double stddev;
	uint count;
	double past_triples_chance;
};

static struct tuple_stats find_unresolved_small_tuples(struct hashcontext *ctx,
						       uint64_t *params, uint n,
						       struct hash_tuples *dest,
						       uint max_size,
						       bool silent)
{
	uint j;
	uint32_t hash_mask = (1 << ctx->bits) - 1;
	uint n_bits = n + BASE_N - 1;
	uint n_hashes = 1 << n_bits;
	struct hash_big_tuple *tuples = calloc(n_hashes, sizeof(tuples[0]));
	uint *size_counts = calloc(MAX_SMALL_TUPLE + 2, sizeof(uint));
	if (max_size == 0){
		max_size = MAX_SMALL_TUPLE;
	} else {
		max_size = MIN(max_size, MAX_SMALL_TUPLE);
	}
	dest->max_size = max_size;
	struct tuple_stats stats = {2, 0, 0, 0, 0, 0};

	for (j = 0; j < ctx->n; j++) {
		uint64_t raw = ctx->data[j].raw_hash;
		uint32_t hash = unmasked_hash(raw, params, n);
		hash &= hash_mask;
		struct hash_big_tuple *p = &tuples[hash];
		if (p->n < max_size) {
			p->array[p->n] = raw;
		} else if (! silent) {
			printf("\033[01;31mmore than %u collisions for hash %x"
			       "\033[00m\n\n", p->n, hash);
		}
		p->n++;
	}

	for (j = 0; j < n_hashes; j++) {
		uint count = MIN(tuples[j].n, max_size + 1);
		size_counts[count]++;
	}
	if (! silent) {
		printf("%4u empty buckets\n", size_counts[0]);
		printf("%4u singletons\n", size_counts[1]);
	}
	bool started = false;
	uint max_count = 1;
	uint64_t sum = 0;
	uint64_t sum2 = 0;
	uint64_t n_tuples = 0;
	for (j = max_size; j >= 2; j--) {
		max_count = MAX(max_count, size_counts[j]);
		if (stats.max == 0 && size_counts[j]) {
			stats.max = j;
		}
	}
	for (j = 2; j <= max_size; j++) {
		uint count = size_counts[j];
		if (count == 0 && ! started) {
			stats.min = j + 1;
			continue;
		}
		started = true;
		sum += count * j;
		sum2 += count * j * j;
		n_tuples += count;

		const char *s = "#############################################";
		if (j <= stats.max && ! silent) {
			uint len = count * strlen(s) / max_count;
			if (count && len == 0) {
				s = ":";
				len = 1;
			}
			printf("%4u tuples of size %-2u "
			       COLOUR(C_DARK_YELLOW, "%.*s\n"), count, j,
			       len, s);
		}
	}
	if (! silent) {
		printf("%4u tuples of size > %u\n", size_counts[max_size + 1],
		       stats.max);
	}
	double scale = 1.0 / n_tuples;
	stats.mean = sum * scale;
	stats.stddev = (sum2 - sum * sum * scale) * scale;
	stats.count = n_tuples;
	dest->tuples[0] = (struct tuple_list){NULL, 0};
	dest->tuples[1] = (struct tuple_list){NULL, 0};

	for (j = 0; j <= max_size; j++) {
		if (size_counts[j]) {
			uint64_t *array = calloc(j * size_counts[j],
						 sizeof(array[0]));
			dest->tuples[j] = (struct tuple_list){array, 0};
		} else {
			dest->tuples[j] = (struct tuple_list){NULL, 0};
		}
	}
	for (j = 0; j < n_hashes; j++) {
		struct hash_big_tuple *p = &tuples[j];
		if (p->n < 2 || p->n > max_size) {
			continue;
		}
		struct tuple_list *tl = &dest->tuples[p->n];
		uint64_t *x = tl->raw + tl->n * p->n;
		for (uint i = 0; i < p->n; i++) {
			x[i] = p->array[i];
		}
		tl->n++;
	}
	for (j = 2; j <= max_size; j++) {
		if (size_counts[j] != dest->tuples[j].n && ! silent) {
			printf("expected %u %u-tuples, got %u\n",
			       size_counts[j], j, dest->tuples[j].n);
		}
	}

	if (stats.max <= 4) {
		/* triples have 6/8 chance of succeeding. quads have 6/16. */
		double n3 = dest->tuples[3].n;
		double n4 = dest->tuples[4].n;
		stats.past_triples_chance = exp(n4 * log(6./16.) +
						n3 * log(6./8.));
	}

	free(tuples);
	free(size_counts);
	return stats;
}


static void free_tuple_data(struct hash_tuples *tuples)
{
	int i;
	for (i = 0; i <= tuples->max_size; i++) {
		if (tuples->tuples[i].raw) {
			free(tuples->tuples[i].raw);
		}
	}
}


static struct tuple_stats get_tuple_stats(struct hashcontext *ctx,
					  uint64_t *params, uint n)
{
	struct hash_tuples tuples;
	struct tuple_stats stats = find_unresolved_small_tuples(ctx, params,
								n, &tuples, 0,
								true);
	free_tuple_data(&tuples);
	return stats;
}


/* count set bits for each rotate */
static inline void count_tuple_collisions_8bit(uint64_t *tuple,
					       uint size,
					       uint64_t param,
					       uint8_t ones8[64])
{
	/*XXX go to 8 bit counters, use aligned array,
	  encourage SSE for compare */
	uint i, j;
	uint64_t p;
	uint64_t mul = MR_MUL(param);
	uint64_t *ones = (uint64_t *)ones8;
	ones[0] = ones[1] = ones[2] = ones[3] = 0ULL;
	ones[4] = ones[5] = ones[6] = ones[7] = 0ULL;
	for (j = 0; j < size; j++) {
		uint64_t r = tuple[j] * mul;
		for (i = 0; i < 8; i++) {
			p = r >> (i * 8);
			p &= 0xffULL;
			p |= p << 28ULL;
			p &= 0x0000000f0000000fULL;
			p |= p << 14ULL;
			p &= 0x0003000300030003ULL;
			p |= p << 7ULL;
			p &= 0x0101010101010101ULL;
			ones[i] += p;
		}
	}
}


static uint do_squashing_round(struct hashcontext *ctx,
			       struct multi_rot *c,
			       uint64_t attempts,
			       uint n, uint max,
			       uint32_t best_score)
{
	uint h, i, k;
	uint64_t j;
	uint64_t best_param = 0;
	uint64_t *params = c->params;
	struct hash_tuples tuples;
	uint n_bits = n + BASE_N - 1;

	uint min, mean;
	max = MIN(max, MAX_SMALL_TUPLE);
	struct tuple_stats stats = find_unresolved_small_tuples(ctx, params,
								n, &tuples,
								max, false);
	min = stats.min;
	max = stats.max;
	mean = stats.mean;
	uint32_t mask = MR_MASK(n);

	attempts /= 20;
	attempts *= MAX(MIN(stats.mean, 25), 5);

	printf("making %'lu attempts. mask %u max %u min %u "
	       "mean %.1f dev %.1f\n",
	       attempts, mask, max, min, stats.mean, stats.stddev);

	reset_close_params(ctx);
	START_TIMER(squashing);

	uint64_t past_half_way = 0;
	uint64_t short_cuts = 0;
	uint param_trick = 0;
	uint64_t param = 0;
	uint last_score = UINT_MAX;
	for (j = 0; j < attempts; j++) {
		/* we don't need to calculate the full hash */
		param = next_param(ctx, j, n + 1, best_param,
				   param, last_score,
				   &param_trick);
		param &= ~MR_ROT_MASK;
		struct tuple_list t;
		uint8_t ones[64] __attribute__((__aligned__(ALIGNMENT)));
		uint scores[64] __attribute__((__aligned__(ALIGNMENT)));
		bool short_cut_exit = false;
		memset(scores, 0, sizeof(scores));
		for (k = max; k >= min; k--) {
			short_cut_exit = false;
			uint ones_target = (k + 1) / 2;
			t = tuples.tuples[k];
			for (i = 0; i < t.n; i++) {
				count_tuple_collisions_8bit(t.raw + i * k,
							    k,
							    param,
							    ones);
				for (h = 0; h < 64; h++) {
					ones[h] = MAX(ones[h], k - ones[h]) - ones_target;
				}
				for (h = 0; h < 64; h++) {
					uint x = ones[h];
					scores[h] += k * ((1 << MIN(x * 3, 20)) >> 1);
				}
 			}
			if (k < max && k > min) {
				short_cut_exit = true;
				for (h = 0; h < 64; h++) {
					if (scores[h] < best_score) {
						short_cut_exit = false;
						break;
					}
				}
				if (short_cut_exit) {
					/* stop looking */
					break;
				}
			}
			if (k == mean) {
				past_half_way++;
			}
		}
		if (short_cut_exit) {
			short_cuts++;
			last_score = UINT_MAX;
			continue;
		}

		uint64_t rotate = n_bits;
		last_score = scores[0];
		for (h = 0; h < 64; h++) {
			if (scores[h] < best_score) {
				printf("new squashing best %16lx & 1«%-2lu at "
				       "%'lu: score %u (%s)\n",
				       MR_MUL(param), rotate, j, scores[h],
				       trick_names[param_trick]);
				best_score = scores[h];
				best_param = param + rotate * MR_ROT_STEP;
				if (best_score == 0) {
					printf("squashing has succeeded!\n");
					goto win;
				}
			}
			rotate = MIN(rotate - 1, 63);
			last_score = MIN(scores[h], last_score);
		}
	}
  win:
	printf("past half_way %'lu times\n", past_half_way);
	printf("short_cuts: %'lu\n", short_cuts);
	params[n] = best_param;
	add_db_param(ctx,  best_param);
	PRINT_TIMER(squashing);
	uint best_collisions = test_params_running(ctx, params, n + 1,
						   UINT_MAX);
	printf("best score %u; collisions %u\n",
	       best_score, best_collisions);

	free_tuple_data(&tuples);
	return best_collisions;
}



static uint do_penultimate_round(struct hashcontext *ctx,
				 struct multi_rot *c,
				 uint64_t attempts,
				 uint n,
				 uint best_collisions)
{
	uint i;
	uint64_t j;
	uint collisions = UINT_MAX;
	uint64_t best_param = 0;
	uint64_t *params = c->params;
	struct hash_tuples tuples;
	uint n_bits = n + BASE_N - 1;
	uint64_t past_triples = 0;
	struct tuple_stats stats = find_unresolved_small_tuples(ctx, params, n,
								&tuples, 4, false);
	if (stats.max > 4) {
		printf(COLOUR(C_RED, "max size %u\n"), stats.max);
		return UINT_MAX;
	}

	attempts *= MAX(1,
			(uint64_t)sqrt((1.00 /
					(3e-4 + stats.past_triples_chance))));
	printf("making %'lu attempts\n", attempts);
	printf("past_triples_chance is %.3e\n",
	       stats.past_triples_chance);
	reset_close_params(ctx);

	/* if there are a lot going through triples, it is better to do the
	   64-way scan of pairs */

	bool parallel_pair_search = (stats.past_triples_chance > 3.0 / 64);
	if (parallel_pair_search) {
		printf("using parallel penultimate search\n");
	}

	START_TIMER(penultimate);
	uint param_trick = 0;
	uint64_t param = 0;
	for (j = 0; j < attempts; j++) {
		/* we don't need to calculate the full hash */
		param = next_param(ctx, j, n + 1, best_param,
				   param, collisions,
				   &param_trick);
		uint64_t non_collisions = ~0ULL;
		uint64_t mul = MR_MUL(param);
		param &= ~MR_ROT_MASK;
		uint64_t a, b, c, d;
		uint64_t ab, ac, ad, bc, bd, cd, abcd;

		/* first check the 4s then the 3s then count the 2s. */
		struct tuple_list t = tuples.tuples[4];
		for (i = 0; i < t.n; i++) {
			a = t.raw[i * 4    ] * mul;
			b = t.raw[i * 4 + 1] * mul;
			c = t.raw[i * 4 + 2] * mul;
			d = t.raw[i * 4 + 3] * mul;
			ab = a ^ b;
			ac = a ^ c;
			ad = a ^ d;
			bc = b ^ c;
			bd = b ^ d;
			cd = c ^ d;

			/* We need each one to not collide with 2 others.

			   if ab is a non-collision, then cd must also be, or
			   either a or b would collide with both of them.
			   (likewise, if ab is a collision, then so must cd be).

			 */
			abcd = ((ab & cd) |
				(ac & bd) |
				(ad & bc));
			non_collisions &= abcd;
		}
		if (non_collisions == 0) {
			/* no rotate of this mul works, try with another */
			collisions = UINT_MAX;
			continue;
		}
		t = tuples.tuples[3];
		for (i = 0; i < t.n; i++) {
			a = t.raw[i * 3    ] * mul;
			b = t.raw[i * 3 + 1] * mul;
			c = t.raw[i * 3 + 2] * mul;
			ab = a ^ b;
			ac = a ^ c;
			non_collisions &= (ab | ac);
		}
		if (non_collisions == 0) {
			collisions = UINT_MAX;
			continue;
		}
		past_triples++;
		/* at this point we have met the mandatory criteria, and
		   increased the number of pairs for the next round by
		   2 * n_quads + 1 * n_triples.

		   we want to reduce the number of pairs in this round as much
		   as possible, so check the good rotates found in the
		   quad/triple check for pair elimination */
		uint64_t p = param;
		uint64_t nc = non_collisions;
		struct tuple_list pairs = tuples.tuples[2];
		if (parallel_pair_search) {
			collisions = test_all_pairs_all_rot(&p, pairs, n + 1,
							    best_collisions, nc);
			if (collisions < best_collisions) {
				best_collisions = collisions;
				best_param = p;
				printf("new penultimate best "
				       "%15lx »%-2lu at %'lu:"
				       " collisions %u (%s)\n",
				       MR_MUL(p), MR_ROT(p), j, collisions,
				       trick_names[param_trick]);
				if (collisions == 0) {
					goto win;
				}
			}
		} else {
			uint rotate = n_bits;
			while (nc) {
				if (nc & 1) {
					p = param + rotate * MR_ROT_STEP;
					collisions = test_all_pairs(p,
								    pairs,
								    n + 1);
					if (collisions < best_collisions) {
						best_collisions = collisions;
						best_param = p;
						printf("new penultimate best "
						       "%15lx »%-2lu at %'lu:"
						       " collisions %u (%s)\n",
						       MR_MUL(p), MR_ROT(p), j,
						       collisions,
						       trick_names[param_trick]);
						if (collisions == 0) {
							goto win;
						}
					}
				}
				nc >>= 1;
				rotate = MIN(rotate - 1, 63);
			}
		}
	}
  win:
	printf("past triples %'lu times (freq. %.2e; predicted %.2e)\n",
	       past_triples, past_triples / (attempts * 64.0),
	       stats.past_triples_chance);

	if (best_param != 0) {
		params[n] = best_param;
		add_db_param(ctx,  best_param);
	} else {
		printf(COLOUR(C_RED, "NEVER past triples; NO best param\n"));
		best_collisions = UINT_MAX;
	}
	PRINT_TIMER(penultimate);
	best_collisions += 2 * tuples.tuples[4].n + tuples.tuples[3].n;
	uint best_collisions2 = test_params_running(ctx, params, n,
						    best_collisions);
	printf("best collisions %u; recalculated %u\n",
	       best_collisions, best_collisions2);

	free_tuple_data(&tuples);
	return best_collisions;
}


static uint do_last_round(struct hashcontext *ctx,
			  struct multi_rot *c,
			  uint64_t attempts,
			  uint64_t n_params)
{
	uint i;
	uint64_t j;
	uint collisions = UINT_MAX;
	uint best_collisions = UINT_MAX;
	uint64_t best_param = 0;
	uint64_t *params = c->params;
	struct hash_tuples tuples;
	reset_close_params(ctx);

	find_unresolved_small_tuples(ctx, params, n_params - 1,
				     &tuples, 2, false);

	struct tuple_list pairs = tuples.tuples[2];

	printf("There are %u unresolved pairs\n", pairs.n);
	attempts *= 3;

	START_TIMER(last);
	uint param_trick = 0;
	uint64_t p;
	if (pairs.n < 64 &&
	    (1UL << pairs.n) < attempts * 6400UL) {
		uint64_t exact_attempts = attempts * 500;
		uint best_run = 0;
		printf("trying for exact solution with %'lu attempts (64 way parallel)\n",
		       exact_attempts);

		/* Test all rotations at once:

		   XOR the two multiplies.
		   anti-collisions are 1s.
		   AND the anti-collisions of pairs.
		   if there is a one bit, that rotation is good.
		 */
		for (j = 0; j < exact_attempts; j++) {
			/* we don't need to calculate the full hash */
			p = next_param(ctx, j, n_params, best_param,
				       p, collisions,
				       &param_trick);
			uint64_t non_collisions = (uint64_t)-1ULL;
			for (i = 0; i < pairs.n; i++) {
				uint64_t raw_a = pairs.raw[i * 2];
				uint64_t raw_b = pairs.raw[i * 2 + 1];
				uint64_t rot_map = test_pair_all_rot(p, raw_a, raw_b,
								     n_params);

				non_collisions &= rot_map;
				if (non_collisions) {
					continue;
				}
				if (i > best_run) {
					best_param = p;
					best_run = i;
					printf("new best run %15lx  "
					       "at %'lu: %u pairs "
					       "(%s)\n",
					       MR_MUL(p), j, i,
					       trick_names[param_trick]);
				}
				break;
			}
			collisions = pairs.n - i;
			if (i == pairs.n) {
				/* we know there is a rotate that works with
				   this multiplier */
				p &= ~MR_ROT_MASK;
				for (i = 0; i < 64; i++) {
					collisions = test_all_pairs(p, pairs,
								    n_params);

					if (collisions == 0) {
						best_param = p;
						printf("WINNING run %15lx »%-2lu at %'lu\n",
						       MR_MUL(p), MR_ROT(p), j);
						goto win;
					}
					p += MR_ROT_STEP;
				}
				printf("we thought we has a winning param, but no!"
				       "%15lx »?? at %'lu\n",
				       MR_MUL(p), j);
			}
		}
	}
	if (pairs.n > 256) {
		printf("the situation is really hopeless. stopping\n");
		return UINT_MAX;
	}

	printf("trying for an inexact solution with %'lu attempts\n",
	       attempts);

	for (j = 0; j < attempts; j++) {
		/* we don't need to calculate the full hash */
		p = next_param(ctx, j, n_params, best_param,
			       p, collisions,
			       &param_trick);
		collisions = test_all_pairs_all_rot(&p, pairs, n_params,
						    best_collisions, 0);

		if (collisions < best_collisions) {
			best_collisions = collisions;
			best_param = p;
			printf("new final round best %15lx »%-2lu at "
			       "%'lu: collisions %u (%s)\n",
			       MR_MUL(p), MR_ROT(p), j, collisions,
			       trick_names[param_trick]);
			if (collisions == 0) {
				printf("found a winner after %'lu\n", j);
				break;
			}
		}
	}
  win:
	params[n_params - 1] = best_param;
	add_db_param(ctx,  best_param);
	PRINT_TIMER(last);
	best_collisions = test_params_running(ctx, params, n_params,
					      best_collisions);

	free_tuple_data(&tuples);
	return best_collisions;

}


static uint do_l2_round(struct hashcontext *ctx,
			struct multi_rot *c,
			uint64_t attempts,
			uint n)
{
	uint64_t j;
	uint collisions;
	uint64_t collisions2 = UINT_MAX;
	uint64_t best_collisions2 = 0;
	uint64_t best_param = 0;
	uint64_t best_error;
	uint64_t *params = c->params;
	uint param_trick = 0;
	reset_close_params(ctx);
	START_TIMER(l2);
	best_error = calc_best_error(ctx, n);
	best_param = 0;
	best_collisions2 = UINT64_MAX;
	printf("making %'lu attempts\n", attempts);

	for (j = 0; j < attempts; j++) {
		params[n] = next_param(ctx, j, n, best_param,
				       params[n], collisions2,
				       &param_trick);
		collisions2 = test_params_with_l2_running(
			ctx,
			params,
			n + 1);
		if (collisions2 < best_collisions2) {
			best_collisions2 = collisions2;
			best_param = params[n];
			printf("new best %15lx »%-2lu at %'lu: "
			       "err %lu > %lu; diff %lu (%s)\n",
			       MR_MUL(best_param), MR_ROT(best_param),
			       j, collisions2, best_error,
			       collisions2 - best_error,
			       trick_names[param_trick]);
			if (collisions2 == best_error) {
				printf("found good candidate after %'lu\n", j);
				collisions = test_params_running(ctx,
								 params,
								 n + 1,
								 UINT_MAX);
				if (collisions == 0) {
					printf("we seem to be finished in "
					       "round %d!\n", n);
					goto done;
				}
				break;
			}
		}
	}
  done:
	params[n] = best_param;
	add_db_param(ctx,  best_param);
	collisions = test_params_running(ctx, params,
					 n + 1,
					 UINT_MAX);
	update_running_hash(ctx, params, n + 1);
	PRINT_TIMER(l2);
	return collisions;
}



static void retry(struct hashcontext *ctx,
		  struct multi_rot *c,
		  uint attempts,
		  uint n_params,
		  uint rounds,
		  uint max_tuple_size,
		  bool do_penultimate)
{
	uint64_t *params = c->params;
	uint i;
	struct hash_tuples tuples;
	static struct tuple_stats stats;
	struct multi_rot orig = *c;
	orig.params = calloc(ctx->n_params, sizeof(uint64_t));
	memcpy(orig.params, c->params, ctx->n_params * sizeof(uint64_t));

	uint target = test_params_l2(ctx, c->params, n_params);
	uint worsts[n_params];
	memset(worsts, 0, sizeof(worsts));
	uint prev_j = n_params;
	for (i = 0; i < rounds; i++) {
		printf(COLOUR(C_GREEN,
			      "-------- round %u ---------------------\n"),
		       i);
		uint mean_score;
		uint j = find_worst_param(ctx, c, n_params, &mean_score);
		worsts[j]++;
		if (j == prev_j) {
			j = 1 + rand64(ctx->rng) % (n_params - 1);
		}
		printf("retrying param %u\n", j);
		prev_j = j;
		reorder_params(c, j, n_params - 1);
		uint64_t victim = params[n_params - 1];

		START_TIMER(retry);

		update_running_hash(ctx, params, n_params - 1);

		/* find the tuples just for the stats */
		stats = find_unresolved_small_tuples(ctx, params, n_params - 1,
						     &tuples, 0, true);
		free_tuple_data(&tuples);

		if (stats.max > max_tuple_size) {
			printf(COLOUR(C_RED,
				      "SKIPPING max tuple %u\n"), stats.max);
			reorder_params(c, j, n_params - 1);
			continue;
		}
		if (stats.max == 2) {
			do_last_round(ctx, c, attempts, n_params);
		} else if (stats.max <= 4 && do_penultimate &&
			   attempts > 1.0 / stats.past_triples_chance) {
			printf(COLOUR(C_YELLOW,
				      "using penultimate method\n"));
			do_penultimate_round(ctx, c, attempts * 2,
					     n_params - 1,
					     UINT_MAX);
		} else {
			do_squashing_round(ctx, c, attempts,
					   n_params - 1,
					   stats.max,
					   UINT_MAX);
		}
		if (params[n_params - 1] == victim) {
			reorder_params(c, j, n_params - 1);
			printf(COLOUR(C_BLUE,
				      "nothing new here\n"));
			PRINT_TIMER(retry);
			continue;
		}

		uint collisions = describe_hash(ctx, c, &orig, n_params, false);
		uint score = test_params_l2(ctx, params, n_params);
		if (score > target) {
			printf("collisions %u, score %u target %u\n",
			       collisions, score, target);
			params[n_params - 1] = victim;
		} else if (score == target) {
			uint new_mean_score;
			printf(COLOUR(C_CYAN,
				      "equal best score %u; param %lx\n"),
			       score, params[n_params - 1]);
			uint k = find_worst_param(ctx, c, n_params, &new_mean_score);
			if (k != j || new_mean_score < mean_score) {
				target = score;
				printf(COLOUR(C_BLUE,
					      "using new param\n"));
			} else {
				params[n_params - 1] = victim;
				printf(COLOUR(C_BLUE,
					      "keeping old param\n"));
			}
		} else {
			printf(COLOUR(C_CYAN,
				      "new best score %u; param %lx\n"),
			       score, params[n_params - 1]);
			target = score;
		}
		reorder_params(c, j, n_params - 1);

		uint target2 = test_params_l2(ctx, c->params, n_params);
		if (target2 != target) {
			printf(COLOUR(C_MAGENTA,
				      "target mismatch; expected %u, got %u\n"),
			       target, target2);
			return;
		}

		PRINT_TIMER(retry);
	}
	describe_hash(ctx, c, &orig, n_params, false);

	for (i = 0; i < n_params; i++) {
		printf("param %u was worst %u times\n", i, worsts[i]);
	}
	free(orig.params);

}

static void init_multi_rot(struct hashcontext *ctx,
			   struct multi_rot *c,
			   uint64_t n_candidates,
			   uint64_t post_squash_retry,
			   uint64_t penultimate_retry,
			   uint quick_early_params)
{
	uint i;
	uint best;
	uint64_t attempts;
	uint64_t *params = c->params;
	struct tuple_stats stats;
	update_running_hash(ctx, params, 0);

	attempts = n_candidates * 20UL;
	if (quick_early_params) {
		attempts = 1000;
	}

	best = do_l2_round(ctx, c, attempts, 0);
	if (best == 0) {
		printf("success in first round!\n");
		c->collisions = best;
		return;
	}
	if (ctx->n_params > 2) {
		for (i = 1; i < ctx->n_params - 2; i++) {
			attempts = n_candidates;
			if (i < quick_early_params) {
				attempts = 1000;
			}
			printf("Beginning round %d (%d bits)\n",
			       i + 1, i + BASE_N);

			stats = get_tuple_stats(ctx, params, i);

			printf("max is %u; min is %u\n", stats.max, stats.min);
			if (stats.max > 1 << (ctx->bits - i - BASE_N + 1)) {
				printf(COLOUR(C_RED,
					      "the situation is hopeless\n"));
			}
			bool use_penultimate = false;
			if (stats.past_triples_chance != 0) {
				if (stats.past_triples_chance > 1.0 / attempts) {
					use_penultimate = true;
					printf("using penultimate method for "
					       "round %u. attempts %lu "
					       "expected interval %.1f\n",
					       i, attempts * 3,
					       1.0 / stats.past_triples_chance);
				} else {
					printf("not using penultimate method. "
					       "attempts %lu "
					       "past threes f %.1g\n",
					       attempts,
					       stats.past_triples_chance);
				}
			}
			if (stats.max > MAX_SMALL_TUPLE) {
				best = do_l2_round(ctx, c, attempts, i);
			} else if (stats.max <= 4 && use_penultimate) {
				do_penultimate_round(ctx, c, attempts * 3,
						     i, UINT_MAX);
			} else {
				best = do_squashing_round(ctx, c, attempts, i,
							  stats.max, UINT_MAX);
			}
			printf("best %u\n", best);
			if (best == 0) {
				best = test_params_running(ctx, params, i,
							   UINT_MAX);
				if (best == 0) {
					printf("early success in round %u\n",
					       i);
					c->collisions = 0;
					return;
				}
			}
		}

		if (post_squash_retry) {
			retry(ctx, c, n_candidates, ctx->n_params - 2,
			      post_squash_retry, 20, true);
		}

		stats = get_tuple_stats(ctx, params, ctx->n_params - 2);
		if (stats.max > 4) {
			printf("the situation is hopeless. stopping\n");
			return;
		}
		/* special cases for the last two rounds */
		if (stats.max == 2) {
			printf("penultimate hash only pairs! "
			       "using do_last_round\n");
			do_last_round(ctx, c, n_candidates, ctx->n_params - 1);
		} else {
			do_penultimate_round(ctx, c, n_candidates * 2,
					     ctx->n_params - 2, UINT_MAX);
		}
		if (penultimate_retry) {
			retry(ctx, c, n_candidates, ctx->n_params - 1,
			      penultimate_retry, 20, true);
		}
	}
	if (ctx->n_params > 1) {
		c->collisions = do_last_round(ctx, c, n_candidates, ctx->n_params);
	} else {
		c->collisions = best;
	}
}


static void print_c_code(struct hashcontext *ctx,
			 struct multi_rot *c)
{
	int i;
	printf("/*** *** *** *** ***/\n");
	printf("uint hash(const char *string)\n");
	printf("{\n");
	printf("\tconst char *s;\n");
	if (ctx->hash_id == HASH_FNV64) {
		printf("\tuint64_t h = %luUL;\n", FNV64_SEED);
	} else if (ctx->hash_id == HASH_DJB) {
		printf("\tuint32_t h = %u;\n", DJB_SEED);
	} else {
		printf("\tuint32_t h = %u;\n", FNV32_SEED);
	}
	printf("\tuint r = 0;\n");
	printf("\tfor (s = string; s != '\\0'; s++) {\n");
	printf("\t\tuint8_t c = (uint8_t)*s;\n");
	if (ctx->case_insensitive) {
		printf("\t\tc &= 0x5f;\n");
	}
	printf("\t\th ^= c;\n");

	if (ctx->hash_id == HASH_FNV64) {
		printf("\t\th *= %luUL;\n", FNV64);
	} else if (ctx->hash_id == HASH_DJB) {
		printf("\t\th *= 33;\n");
	} else {
		printf("\t\th *= %u;\n", FNV32);
	}
	printf("\t}\n");
	if (ctx->pre_filter_mask != 0) {
		printf("\th &= 0x%lx;\n", ctx->pre_filter_mask);
	}

	for (i = 0; i < ctx->n_params; i++) {
		uint64_t x = c->params[i];
		uint64_t mul = MR_MUL(x);
		uint64_t rot = MR_ROT(x);
		uint mask = MR_MASK(i);
		/* XXX untested */
		if (rot >= i) {
			printf("\tr |= ((h * %#17lxULL) >> %2lu) & %#6x;\n",
			       mul, 64 - rot, mask);
		} else {
			/* we need to shift backwards */
			printf("\tr |= ((h * %#17lxULL) << %2lu) & %#6x;\n",
			       mul, rot, mask);
		}
	}
	printf("\treturn r;\n");
	printf("}\n");
}


static struct hashcontext *new_context(const char *filename, uint bits,
				       struct rng *rng, const char *db_name,
				       bool case_insensitive,
				       uint hash_id,
				       bool pre_filter_bits)
{
	struct strings strings = load_strings(filename);
	struct hashdata *data = new_hashdata(&strings,
					     case_insensitive,
					     hash_id);
	free(strings.strings); /* the (char**), not the (char*) */
	struct hashcontext *ctx = malloc(sizeof(*ctx));

	ctx->n_params = bits + 1 - BASE_N;
	uint16_t *hits = calloc((1 << bits), sizeof(hits[0]));

	ctx->data = data;
	ctx->n = strings.n_strings;
	ctx->hits = hits;
	ctx->bits = bits;
	ctx->rng = rng;
	ctx->hash_id = hash_id;
	ctx->string_mem = strings.mem;
	ctx->db_name = db_name;
	ctx->case_insensitive = case_insensitive;
	read_db(ctx, db_name);
	size_t cp_size = N_CLOSE_PARAMS * sizeof(struct close_param);
	ctx->close_params = malloc(cp_size);
	reset_close_params(ctx);

	/* check the hash BEFORE we do the filter bits, because that can take
	   hours */
	if (! check_raw_hash(ctx)) {
		printf("This will never work because the raw hash collides\n");
		printf("(using %s; try something else)\n", hash_names[hash_id]);
		return NULL;
	}
	if (pre_filter_bits) {
		ctx->pre_filter_mask = find_hash_bit_filter(ctx, ctx->data);
		for (uint j = 0; j < ctx->n; j++) {
			ctx->data[j].raw_hash &= ctx->pre_filter_mask;
		}
		if (! check_raw_hash(ctx)) {
			printf("the filter mask makes collisions\n");
			return NULL;
		}
	} else {
		ctx->pre_filter_mask = ~0ULL;
	}
	return ctx;
}

static void free_context(struct hashcontext *ctx)
{
	free(ctx->hits);
	free(ctx->data);
	free(ctx->string_mem);
	free(ctx);
	ctx = NULL;
}

static int find_hash(const char *filename, uint bits,
		     uint n_candidates, struct rng *rng,
		     const char *db_filename,
		     const char *import_text,
		     uint64_t post_squash_retry,
		     uint64_t penultimate_retry,
		     bool c_code,
		     bool case_insensitive,
		     uint quick_early_params,
		     uint hash_id,
		     bool pre_filter_bits)
{
	struct hashcontext *ctx = new_context(filename, bits, rng,
					      db_filename,
					      case_insensitive,
					      hash_id,
					      pre_filter_bits);
	if (ctx == NULL) {
		printf("Context creation fialed for some reason\n");
		exit(1);
	}

	if (! check_raw_hash(ctx)) {
		printf("This will never work because the raw hash collides\n");
		printf("(using %s; try something else)\n", hash_names[hash_id]);
		exit(1);
	}
	if (pre_filter_bits) {
		printf("using %s hash mask %lx -- no collisions\n",
		       hash_names[hash_id],
		       ctx->pre_filter_mask);
	} else {
		printf("using %s hash -- no collisions\n", hash_names[hash_id]);
	}

	if (import_text) {
		import_text_parameters(ctx, import_text);
	}
	struct multi_rot c;
	c.params = calloc(ctx->n_params, sizeof(uint64_t));
	c.collisions = UINT_MAX;
	init_multi_rot(ctx, &c, n_candidates, post_squash_retry,
		       penultimate_retry, quick_early_params);

	if (c.collisions != 0 && false) {
		printf("Final hash from initial search\n");
		describe_hash(ctx, &c, NULL, ctx->n_params, true);
		printf("Retrying\n");
		retry(ctx, &c, n_candidates, ctx->n_params, 20, 4, true);
	}

	struct hashcontext *ctx2 = new_context(filename, bits, rng,
					       NULL, case_insensitive,
					       hash_id,
					       ctx->pre_filter_mask != ~0ULL);

	describe_hash(ctx2, &c, NULL, ctx->n_params, true);
	if (true) {
		remove_unused_bits(ctx2, &c);
		describe_hash(ctx2, &c, NULL, ctx->n_params, true);
	}

	if (c_code) {
		print_c_code(ctx, &c);
	}
	free(c.params);
	free_context(ctx);
	free_context(ctx2);
	return 0;
}


int main(int argc, const char *argv[])
{
	uint i;
	uint64_t bits = 0;
	uint64_t effort = 1000000;
	uint64_t rng_seed = -1ULL;
	uint64_t post_squash_retry = 0ULL;
	uint64_t penultimate_retry = 0ULL;
	int case_insensitive = 0;
	int quick_early_params = 0;
	int c_code = 0;
	char *db = NULL;
	char *import_text = NULL;
	char *hash_function = "FNV32";
	uint hash_id = HASH_FNV32;
	const char *strings = NULL;
	struct rng rng;
	int pre_filter_bits = 0;

	struct argparse argparse;
	struct argparse_option options[] = {
		OPT_HELP(),
		OPT_UINT64('b', "bits", &bits,
			   "find a hash with this many bits"),
		OPT_UINT64('e', "effort", &effort,
			   "make roughly this many attempts per round"),
		OPT_UINT64('r', "rng-seed", &rng_seed,
			   "seed random number generator thus"),
		OPT_STRING('d', "parameter-db", &db,
			   "load/save good params here"),
		OPT_STRING(0, "import-parameters", &import_text,
			   "import parameters implied here"),
		OPT_UINT64('R', "post-squash-retries", &post_squash_retry,
			   "post-squash retry this many times"),
		OPT_UINT64(0, "penultimate-retries", &penultimate_retry,
			   "penultimate retry this many times"),
		OPT_BOOLEAN('C', "c-code", &c_code,
			    "print C code implementing the hash"),
		OPT_BOOLEAN('i', "case-insensitive", &case_insensitive,
			    "ignore case in hashing (ascii only)"),
		OPT_INTEGER('Q', "quick-early-params", &quick_early_params,
			    "skip quickly over this many params"),
		OPT_STRING('H', "hash-function", &hash_function,
			   "raw hash to use (one of fnv32, fnv64, djb)"),
		OPT_BOOLEAN('F', "pre-filter-bits", &pre_filter_bits,
			   "prune redundant hash bits"),
		OPT_END(),
	};
	static const char *const usages[] = {
		"test_argparse [options] [--] <string list>",
		NULL,
	};

	argparse_init(&argparse, options, usages, 0);
	argc = argparse_parse(&argparse, argc, argv);
	if (argc != 1) {
		printf("USAGE: %s\n", usages[0]);
		printf("string list is one string per line.\n");
		return 1;
	} else {
		strings = argv[0];
		printf("using string list: %s.\n", strings);
	}

	if (bits > 24) {
		printf("A %lu bit hash is too big for me! try 24 or less.\n",
		       bits);
		return 2;
	}


	for (i = 0; i < HASH_LAST; i++) {
		if (strcmp(hash_function, hash_names[i]) == 0) {
			hash_id = i;
			break;
		}
	}

	setlocale(LC_NUMERIC, "");

	if (rng_seed == -1ULL) {
		rng_random_init(&rng);
	} else {
		rng_init(&rng, rng_seed);
	}
	return find_hash(strings, bits, effort, &rng, db, import_text,
			 post_squash_retry, penultimate_retry, c_code,
			 case_insensitive, quick_early_params,
			 hash_id, pre_filter_bits);
}
