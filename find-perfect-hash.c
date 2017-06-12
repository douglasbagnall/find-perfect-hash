//gcc -Wall -O3 find-perfect-hash.c -o find-perfect-hash -ggdb -ffast-math -lm -march=native

#define BASE_N 3
#define DETERMINISTIC 0
#define MAX_SMALL_TUPLE 200

#include "find-perfect-hash-helpers.h"
#include <locale.h>
#include "argparse/argparse.h"

struct hashdata {
	const char *string;
	unsigned int stringlen;
	uint64_t raw_hash;
	uint32_t running_hash;
};

struct hashcontext {
	struct hashdata *data;
	uint n;
	uint16_t *hits;
	uint bits;
	struct rng *rng;
	char *string_mem;
	const char *db_name;
	uint64_t *good_params;
	uint n_good_params;
};

struct multi_rot {
	uint64_t *params;
	uint collisions;
};

struct hash_big_tuple {
	uint64_t array[MAX_SMALL_TUPLE];
	uint n;
};

/* tuple size is always known implicitly from without */
struct tuple_list {
	uint64_t *raw;
	uint n;
};

struct hash_tuples {
	struct tuple_list tuples[MAX_SMALL_TUPLE + 1];
	uint max_size;
};


uint N_PARAMS = 0;

//#define FNV 1099511628211UL
#define FNV 16777619

static void init_hash(struct hashdata *hash, const char *string)
{
	uint32_t h = 5381;
	uint32_t h2 = 2166136261;
	const char *s;
	for (s = string; *s != '\0'; s++) {
		uint8_t c = (uint8_t)*s & 0x5f;
		h2 ^= c;
		h2 *= FNV;
		h = ((h << 5) + h) ^ c;
	}
	hash->string = string;
	hash->stringlen = s - string;
	hash->running_hash = 0;
	hash->raw_hash = h2;
}

static struct hashdata *new_hashdata(struct strings *strings)
{
	int i;
	struct hashdata *data = calloc(strings->n_strings,
				       sizeof(data[0]));

	for (i = 0; i < strings->n_strings; i++) {
		init_hash(&data[i], strings->strings[i]);
	}
	return data;
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
		fwrite(ctx->good_params, sizeof(uint64_t),
		       ctx->n_good_params, f);
		//printf("Saved %u good params in %s\n",
		//       ctx->n_good_params, ctx->db_name);
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
			printf("param #%d %lx already saved\n", i, param);
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
	printf("saved param #%u %lx\n", ctx->n_good_params, param);
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


#if 1
#define MR_ROT(x) ((x) & (uint64_t)63)
#define MR_MUL(x) (((x) >> 5) | 1)
#define MR_ROT_STEP 1
#define MR_ROT_MASK 63UL
#define MUL_ROT_TO_PARAM(m, r)((((m) & ~1ULL) << 5) | (r))
#else
#define MR_ROT(x) ((x) >> 58UL)
#define MR_MUL(x) (((x) << 1) | 1)
#define MR_ROT_STEP (1ULL << 58)
#define MR_ROT_MASK (63UL << 58)
#define MUL_ROT_TO_PARAM(m, r)(((m) >> 1) | ((r) << 58))
#endif

#define MR_MASK(i) (((i) == 0) ?					\
		    (1 << BASE_N) - 1 :					\
		    (1 << (BASE_N + (i) - 1)))

#define MR_COMPONENT(x, mul, rot, mask)(ROTATE((x * mul), rot) & mask)

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



static uint find_non_colliding_strings(struct hashcontext *ctx,
				       uint64_t *params, uint n,
				       bool remove_non_colliding)
{
	int j, k;
	uint32_t hash_mask = (1 << ctx->bits) - 1;
	uint16_t *hits = ctx->hits;
	uint worst = 0;
	uint best = UINT_MAX;
	uint n_bits = n + BASE_N - 1;
	/* hash once for the counts */
	for (j = 0; j < ctx->n; j++) {
		uint32_t hash = unmasked_hash(ctx->data[j].raw_hash,
					      params, n);
		hash &= hash_mask;
		hits[hash]++;
	}
	/* hash again to find the unique ones */
	k = 0;
	for (j = 0; j < ctx->n; j++) {
		uint32_t hash = unmasked_hash(ctx->data[j].raw_hash,
					      params, n);
		hash &= hash_mask;
		worst = MAX(worst, hits[hash]);
		best = MIN(best, hits[hash]);

		if (remove_non_colliding && hits[hash] != 1) {
			ctx->data[k] = ctx->data[j];
			k++;
		}
	}

	if (remove_non_colliding) {
		ctx->n = k;
		printf("after %d params (%d bits) dropped %u unique strings, "
		       "leaving %u\n",
		       n, n_bits, j - k, k);
	} else {
		printf("Done %d params (%d bits), not dropping unique strings\n",
		       n, n_bits);
	}
	printf("worst is %u; best is %u\n", worst, best);

	if (worst > 1 << (ctx->bits - n_bits)) {
		printf("the situation is hopeless\n");
	}

	memset(hits, 0, (1 << ctx->bits) * sizeof(hits[0]));
	return worst;
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

static void populate_db(struct hashcontext *ctx)
{
	static const uint64_t known_good[] = {
		MUL_ROT_TO_PARAM(0x04188269d579b41, 41),
		MUL_ROT_TO_PARAM(0x055ee0661539035,  4),
		MUL_ROT_TO_PARAM(0x072f936ef397397, 37),
		MUL_ROT_TO_PARAM(0x0a100976e5aece9, 17),
		MUL_ROT_TO_PARAM(0x0b3bfa87a455f57, 20),
		MUL_ROT_TO_PARAM(0x0f0b48e7f36611d, 26),
		MUL_ROT_TO_PARAM(0x0f9e774af7f03fb, 29),
		MUL_ROT_TO_PARAM(0x106a3a1bc2e6c53, 30),
		MUL_ROT_TO_PARAM(0x127925c6740e3ed, 17),
		MUL_ROT_TO_PARAM(0x17b18b24c617c13, 33),
		MUL_ROT_TO_PARAM(0x1979f230b563735, 29),
		MUL_ROT_TO_PARAM(0x1bea95023ed9d1b, 17),
		MUL_ROT_TO_PARAM(0x1c87c0f29ddf723, 41),
		MUL_ROT_TO_PARAM(0x248f078bc51eb71, 30),
		MUL_ROT_TO_PARAM(0x24cdd66771fd87b, 40),
		MUL_ROT_TO_PARAM(0x25742a1f6dc1d33,  7),
		MUL_ROT_TO_PARAM(0x2ba2b86b604b4e7, 11),
		MUL_ROT_TO_PARAM(0x3327c9d47ab5d01, 29),
		MUL_ROT_TO_PARAM(0x3461b73264c64b3, 44),
		MUL_ROT_TO_PARAM(0x39e5993cb06bb5d, 24),
		MUL_ROT_TO_PARAM(0x3aaffe46faa5409, 43),
		MUL_ROT_TO_PARAM(0x3d4b959ad6b0f11, 27),
		MUL_ROT_TO_PARAM(0x3f6bf5d9e80a759, 23),
		MUL_ROT_TO_PARAM(0x408402f9523bba7, 32),
		MUL_ROT_TO_PARAM(0x4494ea134a45de3, 42),
		MUL_ROT_TO_PARAM(0x45d2ea0a4745b61,  7),
		MUL_ROT_TO_PARAM(0x4bf9dc8c4fa9e77, 34),
		MUL_ROT_TO_PARAM(0x55b4dfda7eee419, 41),
		MUL_ROT_TO_PARAM(0x5c441685af001d7, 29),
		MUL_ROT_TO_PARAM(0x627b75f357d0967, 25),
		MUL_ROT_TO_PARAM(0x648778c8fa85f4f, 34),
		MUL_ROT_TO_PARAM(0x683a5efebe66237, 28),
		MUL_ROT_TO_PARAM(0x6b320a73805869d, 38),
		MUL_ROT_TO_PARAM(0x70361498bc1164b, 35),
		MUL_ROT_TO_PARAM(0x72adddaa227f727, 13),
		MUL_ROT_TO_PARAM(0x7db6e30b60fe3cf, 24),
	};

	for (int i = 0; i < ARRAY_SIZE(known_good); i++) {
		add_db_param(ctx, known_good[i]);
	}
}


static inline uint64_t next_param(struct hashcontext *ctx,
				  uint64_t round,
				  uint64_t *params, uint n_params)
{
	/* these ones work perfect for the first 2 params in
	   ldap_display_names */
	struct rng *rng = ctx->rng;
	if (round < ctx->n_good_params) {
		return ctx->good_params[round];
	}
	uint64_t n_bits = n_params + BASE_N - 1;
	uint64_t p, rot;
	/* low rotates are a bit useless, so we try to select them less
	   often */
	do {
		p = rand64(rng);
		rot = MR_ROT(p) + n_bits;
		n_bits--;
	} while (n_bits < 64 && rot < 10);
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
	static uint8_t ones8[64] __attribute__((__aligned__(ALIGNMENT)));
	static uint64_t *ones = (uint64_t*)ones8;

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


struct size_extrema {
	uint min;
	uint max;
	uint mean;
	uint count;
};

static struct size_extrema find_unresolved_small_tuples(struct hashcontext *ctx,
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
	struct size_extrema size_bounds = {2, max_size, 0};

	for (j = 0; j < ctx->n; j++) {
		uint32_t raw = ctx->data[j].raw_hash;
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
	uint64_t n_tuples = 0;
	uint max_occuring = 0;
	for (j = max_size; j >= 2; j--) {
		max_count = MAX(max_count, size_counts[j]);
		if (max_occuring == 0 && size_counts[j]) {
			max_occuring = j;
		}
	}
	for (j = 2; j <= max_size; j++) {
		uint count = size_counts[j];
		if (count == 0 && ! started) {
			size_bounds.min = j + 1;
			continue;
		}
		started = true;
		sum += count * j;
		n_tuples += count;

		const char *s = "#############################################";
		if (j <= max_occuring && ! silent) {
			uint len = count * strlen(s) / max_count;
			if (count && len == 0) {
				s = ":";
				len = 1;
			}
			printf("%4u tuples of size %-2u "
			       COLOUR(C_DARK_YELLOW, "%.*s\n"), count, j,
			       len, s);
		}
		if (count) {
			size_bounds.max = j;
		}
	}
	if (! silent) {
		printf("%4u tuples of size > %u\n", size_counts[max_size + 1],
		       max_occuring);
	}
	size_bounds.mean = (sum + (n_tuples / 2)) / n_tuples;
	size_bounds.count = n_tuples;
	dest->tuples[0] = (struct tuple_list){NULL, 0};
	dest->tuples[1] = (struct tuple_list){NULL, 0};

	for (j = 0; j <= max_size; j++) {
		uint64_t *array = calloc(j * size_counts[j], sizeof(array[0]));
		dest->tuples[j] = (struct tuple_list){array, 0};
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
	free(tuples);
	free(size_counts);
	return size_bounds;
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
	struct size_extrema size_bounds = find_unresolved_small_tuples(ctx, params,
								       n, &tuples,
								       max, false);
	min = size_bounds.min;
	max = size_bounds.max;
	mean = size_bounds.mean;
	uint32_t mask = MR_MASK(n);

	attempts /= 20;
	attempts *= MAX(MIN(mean, 25), 5);

	printf("making %'lu attempts. mask %u max %u min %u mean %u\n",
	       attempts, mask, max, min, mean);

	START_TIMER(squashing);

	uint64_t past_half_way = 0;
	uint64_t short_cuts = 0;

	for (j = 0; j < attempts; j++) {
		/* we don't need to calculate the full hash */
		uint64_t param = next_param(ctx, j, params, n + 1);
		param &= ~MR_ROT_MASK;
		struct tuple_list t;
		uint8_t ones[64];
		uint scores[64];
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
					scores[h] += (1 << MIN(x, 24)) + x * k - 1;
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
			continue;
		}

		uint64_t rotate = n_bits;
		for (h = 0; h < 64; h++) {
			if (scores[h] < best_score) {
				printf("new squashing best %16lx & 1«%-2lu at %'lu:"
				       " score %u\n",
				       MR_MUL(param), rotate, j, scores[h]);
				best_score = scores[h];
				best_param = param + rotate * MR_ROT_STEP;
				if (best_score == 0) {
					printf("squashing has succeeded!\n");
					goto win;
				}
			}
			rotate = MIN(rotate - 1, 63);
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
	uint collisions;
	uint64_t best_param = 0;
	uint64_t *params = c->params;
	struct hash_tuples tuples;
	uint n_bits = n + BASE_N - 1;
	uint64_t past_triples = 0;
	struct size_extrema size_bounds = find_unresolved_small_tuples(ctx, params, n,
								       &tuples, 4, false);
	if (size_bounds.max > 4) {
		printf(COLOUR(C_RED, "max size %u\n"), size_bounds.max);
		return UINT_MAX;
	}

	/* triples have 6/8 chance of succeeding. quads have 6/16. */
	double past_triples_chance = exp(tuples.tuples[4].n * log(6./16.) +
					 tuples.tuples[3].n * log(6./8.));


	attempts *= (uint64_t) sqrt(1.0 / (3e-4 + past_triples_chance));
	printf("making %'lu attempts\n", attempts);
	printf("past_triples_chance is %.3e\n",
	       past_triples_chance);

	/* if there are a lot going through triples, it is better to do the
	   64-way scan of pairs */

	bool parallel_pair_search = (past_triples_chance > 3.0 / 64);
	if (parallel_pair_search) {
		printf("using parallel penultimate search\n");
	}

	START_TIMER(penultimate);

	for (j = 0; j < attempts; j++) {
		/* we don't need to calculate the full hash */
		uint64_t param = next_param(ctx, j, params, n + 1);
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
			 */
			abcd = ((ab & cd) |
				(ac & bd) |
				(ad & bc));
			non_collisions &= abcd;
		}
		if (non_collisions == 0) {
			/* no rotate of this mul works, try with another */
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
				printf("new penultimate best %15lx »%-2lu at %'lu:"
				       " collisions %u\n",
				       MR_MUL(p), MR_ROT(p), j, collisions);
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
						printf("new penultimate best %15lx »%-2lu at %'lu:"
						       " collisions %u\n",
						       MR_MUL(p), MR_ROT(p), j, collisions);
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
	       past_triples_chance);

	params[n] = best_param;
	add_db_param(ctx,  best_param);
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
			  uint64_t attempts)
{
	uint i;
	uint64_t j;
	uint collisions, best_collisions = UINT_MAX;
	uint64_t best_param = 0;
	uint64_t *params = c->params;
	struct hash_tuples tuples;

	find_unresolved_small_tuples(ctx, params, N_PARAMS - 1,
				     &tuples, 2, false);

	struct tuple_list pairs = tuples.tuples[2];

	printf("There are %u unresolved pairs\n", pairs.n);
	attempts *= 3;

	START_TIMER(last);

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
			uint64_t p = next_param(ctx, j, params, N_PARAMS);
			uint64_t non_collisions = (uint64_t)-1ULL;
			for (i = 0; i < pairs.n; i++) {
				uint64_t raw_a = pairs.raw[i * 2];
				uint64_t raw_b = pairs.raw[i * 2 + 1];
				uint64_t rot_map = test_pair_all_rot(p, raw_a, raw_b,
								     N_PARAMS);

				non_collisions &= rot_map;
				if (non_collisions) {
					continue;
				}
				if (i > best_run) {
					best_param = p;
					best_run = i;
					printf("new best run %15lx  "
					       "at %'lu: %u pairs\n",
					       MR_MUL(p), j, i);
				}
				break;
			}
			if (i == pairs.n) {
				/* we know there is a rotate that works with
				   this multiplier */
				p &= ~MR_ROT_MASK;
				for (i = 0; i < 64; i++) {
					collisions = test_all_pairs(p, pairs,
								    N_PARAMS);

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
		uint64_t p = next_param(ctx, j, params, N_PARAMS);
		collisions = test_all_pairs_all_rot(&p, pairs, N_PARAMS,
						    best_collisions, 0);

		if (collisions < best_collisions) {
			best_collisions = collisions;
			best_param = p;
			printf("new final round best %15lx »%-2lu at %'lu: collisions %u\n",
			       MR_MUL(p), MR_ROT(p), j, collisions);
			if (collisions == 0) {
				printf("found a winner after %'lu\n", j);
				break;
			}
		}
	}
  win:
	params[N_PARAMS - 1] = best_param;
	add_db_param(ctx,  best_param);
	PRINT_TIMER(last);
	best_collisions = test_params_running(ctx, params, N_PARAMS,
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
	uint64_t collisions2, best_collisions2 = 0;
	uint64_t best_param = 0;
	uint64_t best_error;
	uint64_t *params = c->params;
	START_TIMER(l2);
	best_error = calc_best_error(ctx, n);
	best_param = 0;
	best_collisions2 = UINT64_MAX;
	printf("making %'lu attempts\n", attempts);

	for (j = 0; j < attempts; j++) {
		params[n] = next_param(ctx, j, params, n);
		collisions2 = test_params_with_l2_running(
			ctx,
			params,
			n + 1);
		if (collisions2 < best_collisions2) {
			best_collisions2 = collisions2;
			best_param = params[n];
			printf("new best %15lx »%-2lu at %'lu: "
			       "err %lu > %lu; diff %lu\n",
			       MR_MUL(best_param), MR_ROT(best_param),
			       j, collisions2, best_error,
			       collisions2 - best_error);
			if (collisions2 == best_error) {
				printf("found good candidate after %'lu\n", j);
				collisions = test_params_running(ctx,
								 params,
								 n + 1,
								 UINT_MAX);
				if (collisions == 0) {
					printf("we seem to be finished in round %d!\n", n);
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
	static struct size_extrema stats;
	struct multi_rot orig = *c;
	orig.params = calloc(N_PARAMS, sizeof(uint64_t));
	memcpy(orig.params, c->params, N_PARAMS * sizeof(uint64_t));

	uint target = test_params_l2(ctx, c->params, n_params);
	uint worsts[n_params];
	memset(worsts, 0, sizeof(worsts));

	for (i = 0; i < rounds; i++) {
		printf(COLOUR(C_GREEN,
			      "-------- round %u ---------------------\n"),
		       i);
		uint mean_score;
		uint j = find_worst_param(ctx, c, n_params, &mean_score);
		worsts[j]++;
		reorder_params(c, j, n_params - 1);
		uint64_t victim = params[n_params - 1];

		START_TIMER(retry);

		update_running_hash(ctx, params, n_params - 1);

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
			do_last_round(ctx, c, attempts);
#if 1
		} else if (stats.max <= 4 && do_penultimate) {
			do_penultimate_round(ctx, c, attempts * 2,
					     n_params - 1,
					     target * 2);
#endif
		} else {
			do_squashing_round(ctx, c, attempts * 2,
					   n_params - 1,
					   stats.max,
					   UINT_MAX);
		}

		uint collisions = describe_hash(ctx, c, &orig, n_params, false);
		uint score = test_params_l2(ctx, c->params, n_params);
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
			   uint64_t n_candidates)
{
	uint i;
	uint best;
	uint worst = ctx->n;
	uint64_t attempts;
	uint64_t original_n_strings = ctx->n;
	uint64_t *params = c->params;
	update_running_hash(ctx, params, 0);

	best = do_l2_round(ctx, c, n_candidates * 20UL, 0);
	if (best == 0) {
		printf("success in first round!\n");
		c->collisions = best;
		return;
	}
	if (N_PARAMS > 2) {
		for (i = 1; i < N_PARAMS - 2; i++) {
			attempts = n_candidates * original_n_strings / ctx->n;
			worst = find_non_colliding_strings(ctx, params, i,
							   false);

			if (worst > MAX_SMALL_TUPLE) {
				best = do_l2_round(ctx, c, attempts, i);
			} else {
				best = do_squashing_round(ctx, c, attempts, i,
							  worst, UINT_MAX);
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

		worst = find_non_colliding_strings(ctx, params, N_PARAMS - 2, false);
		if (worst > 4) {
			printf("the situation is hopeless. stopping\n");
			return;
		}
		/* special cases for the last two rounds */
		do_penultimate_round(ctx, c, n_candidates * 2,
				     N_PARAMS - 2, UINT_MAX);
	}
	if (N_PARAMS > 1) {
		c->collisions = do_last_round(ctx, c, n_candidates);
	} else {
		c->collisions = best;
	}
}


static struct hashcontext *new_context(const char *filename, uint bits,
				       struct rng *rng, const char *db_name)
{
	struct strings strings = load_strings(filename);
	struct hashdata *data = new_hashdata(&strings);
	free(strings.strings); /* the (char**), not the (char*) */
	struct hashcontext *ctx = malloc(sizeof(*ctx));

	N_PARAMS = bits + 1 - BASE_N;
	uint16_t *hits = calloc((1 << bits), sizeof(hits[0]));

	ctx->data = data;
	ctx->n = strings.n_strings;
	ctx->hits = hits;
	ctx->bits = bits;
	ctx->rng = rng;
	ctx->string_mem = strings.mem;
	ctx->db_name = db_name;
	read_db(ctx, db_name);
	populate_db(ctx);

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
		     const char *db_filename)
{

	struct hashcontext *ctx = new_context(filename, bits, rng,
					       db_filename);

	if (! check_raw_hash(ctx)) {
		printf("This will never work because the raw hash collides\n");
		exit(1);
	}

	struct multi_rot c;
	c.params = calloc(N_PARAMS, sizeof(uint64_t));
	c.collisions = UINT_MAX;
	init_multi_rot(ctx, &c, n_candidates);

	if (c.collisions != 0 && false) {
		printf("Final hash from initial search\n");
		describe_hash(ctx, &c, NULL, N_PARAMS, true);
		printf("Retrying\n");
		retry(ctx, &c, n_candidates, N_PARAMS, 20, 4, true);
	}

	struct hashcontext *ctx2 = new_context(filename, bits, rng,
					       db_filename);

	describe_hash(ctx2, &c, NULL, N_PARAMS, true);
	free(c.params);
	free_context(ctx);
	free_context(ctx2);
	return 0;
}


int main(int argc, const char *argv[])
{
	setlocale(LC_NUMERIC, "");
	uint64_t bits = 0;
	uint64_t effort = 1000000;
	uint64_t rng_seed = -1ULL;
	char *db = NULL;
	const char *strings = NULL;
	struct rng rng;

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

	if (rng_seed == -1ULL) {
		rng_random_init(&rng);
	} else {
		rng_init(&rng, rng_seed);
	}
	return find_hash(strings, bits, effort, &rng, db);
}
