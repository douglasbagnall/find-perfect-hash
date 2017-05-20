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
	uint16_t *hits;
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

#if 1
#define MR_ROT(x) ((x) & (uint64_t)63)
#define MR_MUL(x) (((x) >> 5) | 1)
#define MR_ROT_STEP 1
#define MR_ROT_MASK 63UL
#else
#define MR_ROT(x) ((x) >> 58UL)
#define MR_MUL(x) (((x) << 1) | 1)
#define MR_ROT_STEP (1ULL << 58)
#define MR_ROT_MASK (63UL << 58)
#endif

#define MR_MASK(i) (((i) == 0) ?					\
		    (1 << BASE_N) - 1 :					\
		    ((1 << BITS_PER_PARAM) - 1) << (BASE_N + ((i) - 1) * BITS_PER_PARAM))


#define MR_COMPONENT(x, mul, rot, mask)(((x * mul) >> rot) & mask)

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


static uint test_params(struct hashcontext *ctx,
			uint64_t *params, uint n)
{
	int j;
	uint collisions = 0;
	uint64_t *hits = (uint64_t *)ctx->hits;
	uint32_t hash_mask = (1 << ctx->bits) - 1;
	for (j = 0; j < ctx->n; j++) {
		uint32_t hash = unmasked_hash(ctx->data[j].raw_hash,
					      params, n);
#if BITS_PER_PARAM > 1
		hash &= hash_mask;
#endif
		uint32_t f = hash >> (uint64_t)6;
		uint64_t g = 1UL << (hash & (uint64_t)63);
		collisions += (hits[f] & g) ? 1 : 0;
		hits[f] |= g;
	}
	memset(hits, 0, (hash_mask + 1) / 8);
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
	uint64_t *hits = (uint64_t *)ctx->hits;
	uint32_t hash_mask = (1 << ctx->bits) - 1;
	uint32_t hash;
	uint64_t param = params[n - 1];
	uint64_t rot = MR_ROT(param);
	uint64_t mul = MR_MUL(param);
	uint32_t mask = MR_MASK(n - 1);

	for (j = 0; j < ctx->n; j++) {
		uint32_t comp = MR_COMPONENT(ctx->data[j].raw_hash,
					     mul, rot, mask);

		hash = (ctx->data[j].running_hash ^ comp);
#if BITS_PER_PARAM > 1
		hash &= hash_mask;
#endif
		uint32_t f = hash >> 6;
		uint64_t g = 1UL << (hash & 63);
		collisions += (hits[f] & g) ? 1 : 0;
		hits[f] |= g;
		if (collisions >= best_collisions) {
			break;
		}
	}
	memset(hits, 0, (hash_mask + 1) / 8);
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
#if BITS_PER_PARAM > 1
		hash &= (1 << ctx->bits) - 1;
#endif
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


static uint remove_non_colliding_strings(struct hashcontext *ctx,
					 uint64_t *params, uint n)
{
	int j, k;
	uint32_t hash_mask = (1 << ctx->bits) - 1;
	uint16_t *hits = ctx->hits;
	uint worst = 0;
	uint best = UINT_MAX;
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
		if (hits[hash] != 1) {
			ctx->data[k] = ctx->data[j];
			k++;
			worst = MAX(worst, hits[hash]);
			best = MIN(best, hits[hash]);
		}
	}

	ctx->n = k;
	uint n_bits = n + BASE_N - 1;
	printf("after %d params (%d bits) dropped %u unique strings, leaving %u\n",
	       n, n_bits, j - k, k);
	printf("worst is %u; best is %u\n", worst, best);

	if (worst > 1 << (ctx->bits - n_bits)) {
		printf("the situation is hopeless\n");
	}

	memset(hits, 0, (1 << ctx->bits) * sizeof(hits[0]));
	return worst;
}

static uint64_t calc_best_error(struct hashcontext *ctx, uint n_params)
{
	uint n_bits = BASE_N + n_params * BITS_PER_PARAM;
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

static inline uint64_t next_param(struct rng *rng, uint64_t round,
				  uint64_t *params, uint n_params)
{
	uint64_t p, rot;
	do {
		p = rand64(rng);
		rot = MR_ROT(p);
	} while (rot > 63 - n_params || rot < 5);
	return p;
}


struct hash_pair {
	uint32_t a;
	uint32_t b;
};

static uint find_unresolved_pairs(struct hashcontext *ctx,
				  uint64_t *params, uint n,
				  struct hash_pair **dest)
{
	uint32_t hash_mask = (1 << ctx->bits) - 1;
	struct hash_pair *pairs = calloc(1 << ctx->bits, sizeof(pairs[0]));
	uint i, j;
	uint n_pairs = 0;
	for (j = 0; j < ctx->n; j++) {
		uint32_t hash = unmasked_hash(ctx->data[j].raw_hash,
					      params, n);
		hash &= hash_mask;
		struct hash_pair *p = &pairs[hash];
		if (p->a == 0) {
			p->a = ctx->data[j].raw_hash;
		} else if (p->b == 0) {
			p->b = ctx->data[j].raw_hash;
			n_pairs++;
		} else {
			printf("more than two collisions for hash %x\n",
			       hash);
			*dest = NULL;
			return 0;
		}
	}
	/* shift all the pairs to the beginning */
	i = 0;
	for (j = 0; j < hash_mask + 1; j++) {
		if (pairs[j].b == 0) {
			continue;
		}
		pairs[i] = pairs[j];
		i++;
	}
	*dest = pairs;
	return i;
}


static bool test_pair(uint64_t param,
		      struct hash_pair pair,
		      uint n)
{
	uint64_t rot = MR_ROT(param);
	uint64_t mul = MR_MUL(param);
	uint32_t mask = MR_MASK(n - 1);

	uint32_t a = MR_COMPONENT(pair.a,
				  mul, rot, mask);
	uint32_t b = MR_COMPONENT(pair.b,
				  mul, rot, mask);
	return a != b;
}


static void init_multi_rot(struct hashcontext *ctx,
			   struct multi_rot *c,
			   uint n_candidates)
{
	uint i;
	uint worst = 0;
	uint64_t j, attempts;
	uint collisions, best_collisions = UINT_MAX;
	uint64_t collisions2, best_collisions2 = 0;
	uint64_t best_param = 0;
	uint64_t best_error;
	uint64_t original_n_strings = ctx->n;
	uint64_t *params = c->params;
	update_running_hash(ctx, params, 0);

	for (i = 0; i < N_PARAMS - 1; i++) {
		START_TIMER(l2);
		best_error = calc_best_error(ctx, i);
		best_param = 0;
		best_collisions2 = UINT64_MAX;
		attempts = (uint64_t)n_candidates * original_n_strings / ctx->n;
		printf("making %lu attempts\n", attempts);

		for (j = 0; j < attempts; j++) {
			params[i] = next_param(ctx->rng, j, params, i);
			collisions2 = test_params_with_l2_running(
				ctx,
				params,
				i + 1);
			if (collisions2 < best_collisions2) {
				best_collisions2 = collisions2;
				best_param = params[i];
				printf("new best %15lx »%-2lu at %lu: "
				       "err %lu > %lu; diff %lu\n",
				       MR_MUL(best_param), MR_ROT(best_param),
				       j, collisions2, best_error,
				       collisions2 - best_error);
				if (collisions2 == best_error) {
					printf("found good candidate after %lu\n", j);
					collisions = test_params_running(ctx,
									 params,
									 i + 1,
									 UINT_MAX);
					if (collisions == 0) {
						printf("we seem to be finished in round %d!\n", i);
						goto done;
					}
					break;
				}
			}
		}
		params[i] = best_param;
		update_running_hash(ctx, params, i + 1);
		worst = remove_non_colliding_strings(ctx, params, i + 1);
		PRINT_TIMER(l2);
	}

	best_param = 0;
	/* try extra hard for the last round */
	if (worst > 2) {
		printf("the situation is hopeless. stopping\n");
		goto done;
	}

	struct hash_pair *pairs;
	uint n_pairs = find_unresolved_pairs(ctx, params, N_PARAMS - 1,
					     &pairs);

	printf("There are %u unresolved pairs\n", n_pairs);

	attempts = (uint64_t)n_candidates * original_n_strings / ctx->n * 100;

	printf("making %lu last round attempts\n", attempts);

	START_TIMER(last);

	uint best_run = 0;

	for (j = 0; j < attempts; j++) {
		/* we don't need to calculate the full hash */
		uint64_t p = next_param(ctx->rng, j, params, N_PARAMS);

		for (i = 0; i < n_pairs; i++) {
			if (test_pair(p, pairs[i], N_PARAMS)) {
				continue;
			}
			if (i > best_run) {
				best_param = p;
				best_run = i;
				printf("new best run %15lx »%-2lu at %lu: %u pairs\n",
				       MR_MUL(p), MR_ROT(p), j, i);
			}
			break;
		}
		if (i == n_pairs) {
			best_param = p;
			best_run = i;
			printf("WINNING run %15lx »%-2lu at %lu\n",
			       MR_MUL(p), MR_ROT(p), j);
			break;
		}
	}
	params[N_PARAMS - 1] = best_param;
	PRINT_TIMER(last);

	best_collisions = test_params_running(ctx, params, N_PARAMS,
					      best_collisions);


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
	uint16_t *hits = calloc((1 << size), sizeof(hits[0]));

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
