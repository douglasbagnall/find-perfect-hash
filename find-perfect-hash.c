//gcc -Wall -O3 find-perfect-hash.c -o find-perfect-hash -ggdb -ffast-math -lm -march=native

/*
n 1-bit hashes
calculate pair-wise collisions
(xor pairs of 1 bit hashes) -> 1548 bit vectors 1 means distinction is made
not xor -> 1 means no distinction is made

if no distinction is made by any pair, there is a collision
i.e. AND together the 13 * 12 bit vectors

no.

---
after say n-2 bits (== 11), some of the strings are already unique.

The problem is reduced to a set of smaller problems that can be
dealt with in 2 bits if fewer than 3 collide.

look for evenly spread n-3, n-2, n-1.


 */


#define BITS_PER_PARAM 1
#define DETERMINISTIC 0


#include "find-perfect-hash-helpers.h"

struct hashdata {
	const char *string;
	unsigned int stringlen;
	uint64_t raw_hash;
};

struct hashcontext {
	struct hashdata *data;
	uint n;
	uint64_t *hits;
	uint bits;
	struct rng *rng;
};

struct multi_rot {
	uint64_t *params;
	uint collisions;
};

uint N_PARAMS = 0;

const char *HASH_NAMES[2] = {"djb", "fnv"};

//#define FNV 1099511628211UL
#define FNV 16777619

static void init_hash(struct hashdata *hash, const char *string)
{
	uint32_t h = 5381;
	uint32_t h2 = 2166136261;
	uint32_t *raw32 = (uint32_t *)&hash->raw_hash;
	const char *s;
	for (s = string; *s != '\0'; s++) {
		uint8_t c = (uint8_t)*s & 0x5f;
		h2 ^= c;
		h2 *= FNV;
		h = ((h << 5) + h) ^ c;
	}
	hash->string = string;
	hash->stringlen = s - string;

	raw32[0] = h2;
	raw32[1] = h;
	//hash->raw_hash = h;
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


#define MR_ROT(x) ((x) >> 58ULL)
#define MR_MUL(x) ((x) & ((1ULL << 58ULL) - 1ULL))
#define MR_MASK(i) (((1 << BITS_PER_PARAM) - 1) << (i * BITS_PER_PARAM))

static inline uint32_t hash_component(uint64_t *params, uint i, uint64_t x)
{
	uint64_t param = params[i];
	uint64_t rot = MR_ROT(param);
	uint64_t mul = MR_MUL(param);
	uint32_t mask = MR_MASK(i);
	x *= mul;
	x = ROTATE(x, rot);

	return x & mask;
}

static void describe_hash(struct hashcontext *ctx,
			  struct multi_rot *c)
{
	int i, j;
	int bright = (c->collisions < 80) ? 1 : 0;
	if (c->collisions == 0) {
		printf("\033[01;31m");
	};
	printf("collisions %-3u ", c->collisions);
	printf("\033[00mbits: ");
	for (i = 0; i < N_PARAMS; i++) {
		uint64_t x = c->params[i];
		if (i) {
			printf("\033[01;34m ^ ");
		}
		bool repeat = false;
		for (j = 0; j < N_PARAMS; j++) {
			if (i == j) {
				continue;
			}
			if (x == c->params[j]) {
				repeat = true;
				break;
			}
		}
		if (repeat) {
			printf("\033[01;33m");
		} else {
			printf("\033[0%d;%dm", bright, 36 + (i & 1));
		}
		printf("↻%-2lu ×%013llx ", MR_ROT(x), MR_MUL(x));
		printf("& %04x ", MR_MASK(i));
	}
	printf("\033[00m\n");
}

static uint test_params(struct hashcontext *ctx,
			uint64_t *params, uint n)
{
	int i, j;
	uint collisions = 0;
	uint32_t hash_mask = (1 << ctx->bits) - 1;
	for (j = 0; j < ctx->n; j++) {
		uint32_t hash = 0;
		uint64_t raw_hash = ctx->data[j].raw_hash;
		for (i = 0; i < n; i++) {
			uint32_t comp = hash_component(params, i,
						       raw_hash);
			hash ^= comp;
		}
		hash &= hash_mask;
		uint32_t f = hash >> 6;
		uint64_t g = 1UL << (hash & 63);
		collisions += (ctx->hits[f] & g) ? 1 : 0;
		ctx->hits[f] |= g;
	}
	memset(ctx->hits, 0, (1 << ctx->bits) / 8);
	return collisions;
}


static uint test_params_with_l2(struct hashcontext *ctx,
				uint64_t *params, uint n,
				uint64_t *collisions2)
{
	int i, j;
	uint collisions = 0;
	uint32_t hash_mask = (1 << ctx->bits) - 1;
	uint16_t *hits = (uint16_t *) ctx->hits;
	for (j = 0; j < ctx->n; j++) {
		uint32_t hash = 0;
		uint64_t raw_hash = ctx->data[j].raw_hash;
		for (i = 0; i < n; i++) {
			uint32_t comp = hash_component(params, i,
						       raw_hash);
			hash ^= comp;
		}
		hash &= hash_mask;
		uint16_t h = hits[hash];
		if (h) {
			collisions++;
		}
		hits[hash] = h + 1;
	}

	uint64_t c2 = 0;

	for (i = 0; i <= hash_mask; i++) {
		uint h = hits[i];
		c2 += h * h;
	}
	*collisions2 = c2;
	memset(hits, 0, (1 << ctx->bits) * sizeof(hits[0]));
	return collisions;
}

static uint64_t calc_best_error(struct hashcontext *ctx, uint n_params)
{
	uint n_bits = n_params * BITS_PER_PARAM;
	uint n = 1 << n_bits;
	uint q = ctx->n / n;
	uint r = ctx->n % n;
	uint64_t sum = n * q * q + 2 * q * r + r;
	return sum;
}

static void init_multi_rot(struct hashcontext *ctx,
			   struct multi_rot *c,
			   uint64_t *params,
			   uint n_candidates)
{
	int i, j;
	uint collisions, best_collisions;
	uint64_t collisions2, best_collisions2 = 0;
	uint64_t best_param = 0;
	uint64_t best_error;
	uint64_t *pool = calloc(n_candidates, sizeof(uint64_t));
	for (i = 0; i < n_candidates; i++) {
		pool[i] = rand64(ctx->rng);
	}

	best_collisions = ctx->n + 2;
	best_collisions2 = UINT64_MAX;

	const uint base_n = MIN(3, N_PARAMS * 2 / 3);

	best_error = calc_best_error(ctx, base_n);
	for (j = 0; j < n_candidates; j++) {
		uint64_t p = pool[j];
		for (i = 0; i < base_n; i++) {
			params[i] = p;
		}
		collisions = test_params_with_l2(ctx,
						 params,
						 base_n,
						 &collisions2);
		if (collisions2 < best_collisions2) {
			best_collisions2 = collisions2;
			best_collisions = collisions;
			best_param = p;
			printf("new best candidate at %d: err %lu best %lu diff %lu\n",
			       j, collisions2, best_error, collisions2 - best_error);
			if (collisions2 <= best_error + (best_error >> 8)) {
				printf("found good enough candidate after %d\n", j);
			}
		}
	}

	for (i = 0; i < base_n; i++) {
		params[i] = best_param;
	}

	for (i = base_n; i < N_PARAMS - 1; i++) {
		best_error = calc_best_error(ctx, i + 1);
		best_param = 0;
		best_collisions = ctx->n + 2;
		best_collisions2 = UINT64_MAX;
		for (j = 0; j < n_candidates; j++) {
			params[i] = pool[j];

			collisions = test_params_with_l2(ctx,
							 params,
							 i + 1,
							 &collisions2);
			if (collisions2 < best_collisions2) {
				best_collisions2 = collisions2;
				best_collisions = collisions;
				best_param = params[i];
				printf("new best candidate at %d: collisions %u "
				       "err %lu > %lu diff %lu\n", j, collisions,
				       collisions2, best_error, collisions2 - best_error);
				if (collisions2 == best_error) {
					printf("found good enough candidate after %d\n", j);
					break;
				}
			}
		}
		params[i] = best_param;
	}

	best_param = 0;

	for (j = 0; j < n_candidates; j++) {
		params[N_PARAMS - 1] = pool[j];
		collisions = test_params(ctx,
					 params,
					 N_PARAMS);
		if (collisions < best_collisions) {
			best_collisions = collisions;
			best_param = pool[j];
			printf("new final round best at %d: collisions %u\n",
			       j, collisions);
			if (collisions == 0) {
				printf("found a winner after %d\n", j);
				break;
			}
		}
	}
	free(pool);
	c->params = params;
	c->collisions = best_collisions;
}


static int find_hash(const char *filename, uint bits,
		     uint n_candidates, struct rng *rng)
{
	struct strings strings = load_strings(filename);
	struct hashdata *data = new_hashdata(&strings);
	N_PARAMS = (bits + (BITS_PER_PARAM - 1)) / BITS_PER_PARAM;
	uint size = N_PARAMS * BITS_PER_PARAM;

	uint64_t *hits = calloc((1 << size), 2);

	struct hashcontext ctx = {data, strings.n_strings, hits};

	ctx.bits = bits;
	ctx.rng = rng;

	uint64_t params[N_PARAMS];
	struct multi_rot c;
	init_multi_rot(&ctx, &c, params, n_candidates);

	describe_hash(&ctx, &c);

	free(hits);
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
