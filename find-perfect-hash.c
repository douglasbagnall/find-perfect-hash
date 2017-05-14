//gcc -Wall -O3 find-perfect-hash.c -o find-perfect-hash -ggdb -ffast-math -lm -march=native


#define BITS_PER_PARAM 1
#define BASE_N 4
#define DETERMINISTIC 01


#include "find-perfect-hash-helpers.h"

struct hashdata {
	const char *string;
	unsigned int stringlen;
	uint64_t raw_hash;
	uint32_t running_hash;
};

struct hashcontext {
	struct hashdata *data;
	uint n;
	uint64_t *hits;
	uint bits;
	struct rng *rng;
	char *string_mem;
};

struct multi_rot {
	uint64_t *params;
	uint collisions;
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
	//uint32_t *raw32 = (uint32_t *)&hash->raw_hash;
	//raw32[0] = h2;
	//raw32[1] = h;
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

#if 0
#define MR_ROT(x) ((x) & (uint64_t)63)
#define MR_MUL(x) ((x) >> 6)
#define MR_ROT_STEP 1
#define MR_ROT_MASK 63UL
#else
#define MR_ROT(x) ((x) >> 58UL)
#define MR_MUL(x) (x)
#define MR_ROT_STEP (1ULL << 58)
#define MR_ROT_MASK (63UL << 58)
#endif

#define MR_MASK(i) ((i == 0) ?						\
		    (1 << BASE_N) - 1 :					\
		    ((1 << BITS_PER_PARAM) - 1) << (BASE_N + (i - 1) * BITS_PER_PARAM))


static inline uint32_t hash_component(uint64_t *params, uint i, uint64_t x)
{
	uint64_t param = params[i];
	uint64_t rot = MR_ROT(param);
	uint64_t mul = MR_MUL(param);
	uint32_t mask = MR_MASK(i);
	x *= mul;
	x >>= rot;
	//x = ROTATE(x, rot);

	return x & mask;
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
	if ( a > b) {
		uint temp = a;
		a = b;
		b = temp;
	}
	uint diff = b - a;
	uint64_t *params = c->params;
	uint64_t p1 = params[a];
	uint64_t p2 = params[b];
	uint64_t mul1 = params[a] & ~MR_ROT_MASK;
	uint64_t mul2 = params[b] & ~MR_ROT_MASK;
	uint64_t rot1 = p1 & MR_ROT_MASK;
	uint64_t rot2 = p2 & MR_ROT_MASK;
	uint64_t r1 = MR_ROT(p1);
	uint64_t r2 = MR_ROT(p2);
	printf("reordering %u %u, diff %u r1 %lu r2 %lu\n",
	       a, b, diff, r1, r2);

	if (diff > r1 ||
	    diff + r2 > 63) {
		printf("diff %u r1 %lu r2 %lu\n", diff, r1, r2);
		return false;
	}

	uint64_t new1 = (rot1 - diff * MR_ROT_STEP) & MR_ROT_MASK;
	uint64_t new2 = (rot2 + diff * MR_ROT_STEP) & MR_ROT_MASK;

	params[b] = mul1 | new1;
	params[a] = mul2 | new2;
	return true;
}


static uint test_params(struct hashcontext *ctx,
			uint64_t *params, uint n)
{
	int j;
	uint collisions = 0;
	uint64_t *hits = ctx->hits;
	uint32_t hash_mask = (1 << ctx->bits) - 1;
	for (j = 0; j < ctx->n; j++) {
		uint32_t hash = unmasked_hash(ctx->data[j].raw_hash,
					      params, n);
		hash &= hash_mask;
		uint32_t f = hash >> (uint64_t)6;
		uint64_t g = 1UL << (hash & (uint64_t)63);
		collisions += (hits[f] & g) ? 1 : 0;
		hits[f] |= g;
	}
	memset(hits, 0, (1 << ctx->bits) / 8);
	return collisions;
}


static void describe_hash(struct hashcontext *ctx,
			  struct multi_rot *c)
{
	int i, j;

	printf("bits %u mask %x\n", ctx->bits,
	       (1 << ctx->bits) - 1);

	uint collisions = test_params(ctx, c->params, N_PARAMS);

	if (collisions != c->collisions) {
		printf("\033[01;31m collisions mismatch! \033[00m\n"
		       "presumed %u actual %u\n", c->collisions, collisions);
	}
	if (collisions == 0) {
		printf("\033[01;31m");
	};
	printf("collisions %-3u ", collisions);
	printf("\033[00mbits: ");
	for (i = 0; i < N_PARAMS; i++) {
		uint64_t x = c->params[i];
		uint64_t masked = x  & ~MR_ROT_MASK;
		uint mask = MR_MASK(i);
		if (i) {
			printf("\033[01;34m ^ ");
		}

		bool duplicate = false;
		for (j = 0; j < N_PARAMS; j++) {
			if (i != j &&
			    masked == (c->params[j] & ~MR_ROT_MASK)) {
				duplicate = true;
				break;
			}
		}
		if (duplicate) {
			printf("\033[01;33m");
		} else {
			printf("\033[01;%dm", 36 + (i != 0));
		}
		printf("×%015lx »%-2lu & %04x ",
		       MR_MUL(x), MR_ROT(x), mask);
	}
	printf("\033[00m\n");
}

static uint test_params_running(struct hashcontext *ctx,
				uint64_t *params, uint n,
				uint best_collisions)
{
	int j;
	uint collisions = 0;
	uint64_t *hits = ctx->hits;
	uint32_t hash_mask = (1 << ctx->bits) - 1;
	uint32_t hash;
	for (j = 0; j < ctx->n; j++) {
		uint32_t comp = hash_component(params, n - 1,
					       ctx->data[j].raw_hash);

		hash = (ctx->data[j].running_hash ^ comp) & hash_mask;

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


static uint test_params_with_l2(struct hashcontext *ctx,
				uint64_t *params, uint n,
				uint64_t *collisions2,
				uint64_t best_c2)
{
	int j;
	uint collisions = 0;
	uint32_t hash_mask = (1 << ctx->bits) - 1;
	uint16_t *hits = (uint16_t *) ctx->hits;
	uint64_t c2 = 0;
	for (j = 0; j < ctx->n; j++) {
		uint32_t hash = unmasked_hash(ctx->data[j].raw_hash,
					      params, n);
		hash &= hash_mask;
		uint16_t h = hits[hash];
		c2 += 2 * h + 1;
		if (h) {
			collisions++;
		}
		if (c2 >= best_c2) {
			//printf("hit limit at %d\n", j);
			collisions = UINT_MAX;
			break;
		}
		hits[hash] = h + 1;
	}

	*collisions2 = c2;
	memset(hits, 0, (1 << ctx->bits) * sizeof(hits[0]));
	return collisions;
}


static uint test_params_with_l2_running(struct hashcontext *ctx,
					uint64_t *params, uint n,
					uint64_t *collisions2,
					uint64_t best_c2)
{
	int j;
	uint collisions = 0;
	uint32_t hash_mask = (1 << ctx->bits) - 1;
	uint16_t *hits = (uint16_t *) ctx->hits;
	uint64_t c2 = 0;
	for (j = 0; j < ctx->n; j++) {
		uint32_t comp = hash_component(params, n - 1,
					       ctx->data[j].raw_hash);
		uint32_t hash = (ctx->data[j].running_hash ^ comp) & hash_mask;

		hash &= hash_mask;
		uint16_t h = hits[hash];
		c2 += 2 * h + 1;
		if (h) {
			collisions++;
		}
		if (c2 >= best_c2) {
			//printf("j: %d, c2 %lu\n", j, c2);
			collisions = UINT_MAX;
			break;
		}
		hits[hash] = h + 1;
	}

	*collisions2 = c2;
	memset(hits, 0, (1 << ctx->bits) * sizeof(hits[0]));
	return collisions;
}


static void remove_non_colliding_strings(struct hashcontext *ctx,
					 uint64_t *params, uint n)
{
	int j, k;
	uint32_t hash_mask = (1 << ctx->bits) - 1;
	uint16_t *hits = (uint16_t *) ctx->hits;
	/* hash once for the counts */
	for (j = 0; j < ctx->n; j++) {
		uint32_t hash = running_unmasked_hash(ctx->data[j].raw_hash,
						      ctx->data[j].running_hash,
						      params, n);
		hash &= hash_mask;
		//printf("hash %u mask %u hits %p\n", hash, hash_mask, ctx->hits);
		hits[hash]++;
	}
	/* hash again to find the unique ones */
	k = 0;
	for (j = 0; j < ctx->n; j++) {
		uint32_t hash = running_unmasked_hash(ctx->data[j].raw_hash,
						      ctx->data[j].running_hash,
						      params, n);
		hash &= hash_mask;
		if (hits[hash] != 1) {
			ctx->data[k] = ctx->data[j];
			k++;
		}
	}

	ctx->n = k;
	printf("after %d params dropped %u unique strings, leaving %u\n",
	       n, j - k, k);

	memset(hits, 0, (1 << ctx->bits) * sizeof(hits[0]));
}


static uint64_t calc_best_error(struct hashcontext *ctx, uint n_params)
{
	uint n_bits = BASE_N + n_params * BITS_PER_PARAM;
	uint n = 1 << n_bits;
	uint q = ctx->n / n;
	uint r = ctx->n % n;
	uint64_t sum = n * q * q + 2 * q * r + r;
	return sum;
}

static inline uint64_t next_param(struct rng *rng, uint round,
				  uint64_t *params, uint n_params)
{
	static uint64_t base = 0;
	uint r = round % 48;
	if (r == 0) {
		if (round < n_params * 48) {
			base = params[round / 48] & ~MR_ROT_MASK;
		} else {
			base = rand64(rng) & ~MR_ROT_MASK;
		}
	}
	return base + r * MR_ROT_STEP;
}



static uint find_worst_param(struct hashcontext *ctx,
			     struct multi_rot *c)
{
	uint64_t *params = c->params;
	uint64_t collisions2, best_collisions2;
	uint32_t collisions, best_collisions;
	uint i, j, best_i = 0, best_i2 = 0;
	best_collisions = ctx->n + 2;
	best_collisions2 = UINT64_MAX;

	for (i = 1; i < N_PARAMS; i++) {
		uint64_t p = params[i];
		params[i] = 0;
		collisions = test_params_with_l2(ctx,
						 params,
						 N_PARAMS,
						 &collisions2,
						 UINT64_MAX);
		params[i] = p;

		printf("removing param %u gives collisions %u, "
		       "collisions2 %lu\n", i, collisions, collisions2);

		if (collisions2 < best_collisions2) {
			best_collisions2 = collisions2;
			best_i2 = i;
		}

		if (collisions < best_collisions) {
			best_collisions = collisions;
			best_i = i;
		}
	}
	printf("best collisions: %u without param %u\n",
	       best_collisions, best_i);
	printf("best collisions2: %lu without param %u\n",
	       best_collisions2, best_i2);

	uint64_t params2[N_PARAMS];
	uint64_t orig_collisions2;
	memcpy(params2, params, sizeof(uint64_t) * N_PARAMS);
	uint orig_collisions = test_params_with_l2(ctx,
						   params,
						   N_PARAMS,
						   &orig_collisions2,
						   UINT64_MAX);
	best_collisions2 = orig_collisions2;
	best_collisions = orig_collisions;

	for (i = 1; i < N_PARAMS - 2; i++) {
		bool reordered = reorder_params(c, i, N_PARAMS - 1);
		uint64_t best_param = params[N_PARAMS - 1];
		if (! reordered) {
			printf("failed to reorder %u -> %u\n", i, N_PARAMS - 1);
			continue;
		}
		update_running_hash(ctx, params, N_PARAMS);

		uint attempts = 100000;
		for (j = 0; j < attempts; j++) {
			params[N_PARAMS - 1] = next_param(ctx->rng, j, params, N_PARAMS - 1);
			collisions = test_params_with_l2(
				ctx,
				params,
				N_PARAMS,
				&collisions2,
				best_collisions2);
			//printf("%d: param %lx collisions %u err %lu best %lu\n",
			//      j, params[N_PARAMS -1], collisions, collisions2,
			//      best_collisions2);
			if (collisions2 < best_collisions2) {
				best_collisions2 = collisions2;
				best_collisions = collisions;
				best_param = params[i];
				printf("new best at %d: collisions %u err %lu\n",
				       j, collisions, collisions2);
				if (collisions == 0) {
					printf("we seem to be finished in round %d!\n", i);
					goto done;
				}
			}
		}
		params[N_PARAMS - 1] = best_param;
		reordered = reorder_params(c, N_PARAMS - 1, i);
		if (! reordered) {
			printf("failed to restore %u -> %u\n", i, N_PARAMS - 1);
		}
	}
  done:
	return best_i2;
}

static void init_multi_rot(struct hashcontext *ctx,
			   struct multi_rot *c,
			   uint n_candidates)
{
	uint i, j;
	uint collisions, best_collisions = 0;
	uint64_t collisions2, best_collisions2 = 0;
	uint64_t best_param = 0;
	uint64_t best_error;
	uint64_t original_n_strings = ctx->n;
	uint64_t *params = c->params;

	update_running_hash(ctx, params, 0);

	for (i = 0; i < N_PARAMS ; i++) {
		START_TIMER(l2);
		best_error = calc_best_error(ctx, i);
		best_param = 0;
		best_collisions = ctx->n + 2;
		best_collisions2 = UINT64_MAX;
		uint attempts = MIN(((uint64_t)n_candidates *
				     original_n_strings / ctx->n),
				    UINT_MAX);
		printf("making %u attempts\n", attempts);

		for (j = 0; j < attempts; j++) {
			params[i] = next_param(ctx->rng, j, params, i);
			collisions = test_params_with_l2_running(
				ctx,
				params,
				i + 1,
				&collisions2,
				best_collisions2);
			if (collisions2 < best_collisions2) {
				best_collisions2 = collisions2;
				best_collisions = collisions;
				best_param = params[i];
				double ratio = collisions2 * (1.0 / best_error);
				printf("new best at %d: collisions %u "
				       "err %lu > %lu; diff %lu ratio %.3f\n",
				       j, collisions, collisions2, best_error,
				       collisions2 - best_error, ratio);
				if (collisions == 0) {
					printf("we seem to be finished in round %d!\n", i);
					goto done;
				}
				if (collisions2 == best_error) {
					printf("found good candidate after %d\n", j);
					break;
				}
			}
		}
		params[i] = best_param;
		update_running_hash(ctx, params, i + 1);
		remove_non_colliding_strings(ctx, params, i + 1);
		PRINT_TIMER(l2);
	}
	goto done;
	best_param = 0;
	/* try extra hard for the last round */
	uint attempts = MIN(((uint64_t)n_candidates * original_n_strings / ctx->n),
			    UINT_MAX);
	printf("making %u last round attempts\n", attempts);

	START_TIMER(last);

	for (j = 0; j < attempts; j++) {
		params[N_PARAMS - 1] = next_param(ctx->rng, j, params, N_PARAMS);
		collisions = test_params_running(ctx,
						 params,
						 N_PARAMS,
						 best_collisions);
		if (collisions < best_collisions) {
			best_collisions = collisions;
			best_param = params[N_PARAMS - 1];
			printf("new final round best at %d: collisions %u\n",
			       j, collisions);
			if (collisions == 0) {
				printf("found a winner after %d\n", j);
				break;
			}
		}
	}
	params[N_PARAMS - 1] = best_param;
	PRINT_TIMER(last);

  done:
	c->collisions = best_collisions;
}


struct hashcontext *new_context(const char *filename, uint bits,
				uint n_candidates, struct rng *rng)
{
	struct strings strings = load_strings(filename);
	struct hashdata *data = new_hashdata(&strings);
	free(strings.strings); /* the (char**), not the (char*) */
	struct hashcontext *ctx = malloc(sizeof(*ctx));

	N_PARAMS = 1 + (bits - BASE_N + (BITS_PER_PARAM - 1)) / BITS_PER_PARAM;
	uint size = BASE_N + (N_PARAMS - 1) * BITS_PER_PARAM;
	printf("size %u bits %u\n", size, bits);
	uint64_t *hits = calloc((1 << size), sizeof(uint16_t));

	ctx->data = data;
	ctx->n = strings.n_strings;
	ctx->hits = hits;
	ctx->bits = bits;
	ctx->rng = rng;
	ctx->string_mem = strings.mem;
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
		     uint n_candidates, struct rng *rng)
{

	struct hashcontext *ctx = new_context(filename, bits,
					     n_candidates, rng);

	if (! check_raw_hash(ctx)) {
		printf("This will never work because the raw hash collides\n");
		exit(1);
	}

	struct multi_rot c;
	c.params = calloc(N_PARAMS, sizeof(uint64_t));
	init_multi_rot(ctx, &c, n_candidates);

	struct hashcontext *ctx2 = new_context(filename, bits,
					       n_candidates, rng);


	describe_hash(ctx2, &c);
	find_worst_param(ctx2, &c);
	describe_hash(ctx2, &c);
	free(c.params);
	free_context(ctx);
	free_context(ctx2);
	return 0;
}


int main(int argc, char *argv[])
{
	if (argc < 3) {
		printf("usage: %s <string list> <hash bits>\n\n",
		       argv[0]);
		printf("string list is one string per line.\n");
	}
	printf("got %s %s %s\n",
	       argv[0], argv[1], argv[2]);

	uint bits = strtoul(argv[2], NULL, 10);
	if (bits > 28) {
		printf("A %u bit hash is too big for me! try 28 or less.\n",
		       bits);
		return 2;
	}

	uint n_candidates = strtoul(argv[3], NULL, 10);
	printf("Using %u candidates per round \n", n_candidates);

	struct rng rng;
#if DETERMINISTIC
	rng_init(&rng, 12345);
#else
	rng_random_init(&rng);
#endif
	return find_hash(argv[1], bits, n_candidates, &rng);
}
