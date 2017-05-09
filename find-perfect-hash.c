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
	uint32_t running_hash;
};

struct hashcontext {
	struct hashdata *data;
	uint n;
	uint64_t *hits;
	uint bits;
	struct rng *rng;
	uint running_n;
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

#define MR_ROT(x) ((x) & (uint64_t)63)
//#define MR_MUL(x) ((x) & ((1ULL << 58ULL) - 1ULL))
#define MR_MUL(x) ((x) >> 6)
#define MR_MASK(i) (((1 << BITS_PER_PARAM) - 1) << (i * BITS_PER_PARAM))

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
	if (n > 0) {
		for (i = 0; i < ctx->n; i++) {
			ctx->data[i].running_hash = unmasked_hash(
				ctx->data[i].raw_hash,
				params, n);
		}
	} else {
		for (i = 0; i < ctx->n; i++) {
			ctx->data[i].running_hash = 0;
		}
	}
	ctx->running_n = n;
}


static void describe_hash(struct hashcontext *ctx,
			  struct multi_rot *c)
{
	int i, j;
	if (c->collisions == 0) {
		printf("\033[01;31m");
	};
	printf("collisions %-3u ", c->collisions);
	printf("\033[00mbits: ");
	for (i = 0; i < N_PARAMS; i++) {
		uint64_t x = c->params[i];
		uint mask = MR_MASK(i);
		bool duplicate = false;
		for (j = 0; j < i; j++) {
			if (x == c->params[j]) {
				duplicate = true;
				break;
			}
		}
		if (duplicate) {
			continue;
		}
		for (j = i + 1; j < N_PARAMS; j++) {
			if (x == c->params[j]) {
				mask |= MR_MASK(j);
			}
		}
		if (i) {
			printf("\033[01;34m ^ ");
		}

		if (mask != MR_MASK(i)) {
			printf("\033[01;33m");
		} else {
			printf("\033[01;%dm", 36 + (i & 1));
		}
		printf("↻%-2lu ×%013lx & %04x ",
		       MR_ROT(x), MR_MUL(x), mask);
	}
	printf("\033[00m\n");
}

static uint test_params(struct hashcontext *ctx,
			uint64_t *params, uint n,
			uint best_collisions)
{
	int j;
	uint collisions = 0;
	uint64_t *hits = ctx->hits;
	uint32_t hash_mask = (1 << ctx->bits) - 1;
	for (j = 0; j < ctx->n; j++) {
		uint32_t hash = unmasked_hash(ctx->data[j].raw_hash,
					      params, n);
		hash &= hash_mask;
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

static uint test_params_running(struct hashcontext *ctx,
				uint64_t *params, uint n,
				uint best_collisions)
{
	int j;
	uint collisions = 0;
	uint64_t *hits = ctx->hits;
	uint32_t hash_mask = (1 << ctx->bits) - 1;
	for (j = 0; j < ctx->n; j++) {
		uint32_t hash = running_unmasked_hash(ctx->data[j].raw_hash,
						      ctx->data[j].running_hash,
						      params, n);
		hash &= hash_mask;
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
				uint64_t best_c2,
				bool full)
{
	int j;
	uint collisions = 0;
	uint32_t hash_mask = (1 << ctx->bits) - 1;
	uint16_t *hits = (uint16_t *) ctx->hits;
	uint64_t c2 = 0;
	for (j = 0; j < ctx->n; j++) {
		uint32_t hash;
		if (full) {
			hash = unmasked_hash(ctx->data[j].raw_hash,
					     params, n);
		} else {
			hash = running_unmasked_hash(ctx->data[j].raw_hash,
						      ctx->data[j].running_hash,
						      params, n);
		}
		hash &= hash_mask;
		uint16_t h = hits[hash];
		c2 += 2 * h + 1;
		if (h) {
			collisions++;
		}
		if (c2 >= best_c2) {
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
	printf("%d params dropped %u unique strings, down to %u\n",
	       n, j - k, k);

	memset(hits, 0, (1 << ctx->bits) * sizeof(hits[0]));
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
	uint64_t original_n_strings = ctx->n;

	update_running_hash(ctx, params, 0);

	best_collisions = ctx->n + 2;
	best_collisions2 = UINT64_MAX;

	const uint base_n = MIN(4, N_PARAMS * 3 / 4);

	best_error = calc_best_error(ctx, base_n);

	for (j = 0; j < n_candidates; j++) {
		uint64_t p = rand64(ctx->rng);
		for (i = 0; i < base_n; i++) {
			params[i] = p;
		}
		collisions = test_params_with_l2(ctx,
						 params,
						 base_n,
						 &collisions2,
						 best_collisions2,
						 true);
		if (collisions2 < best_collisions2) {
			best_collisions2 = collisions2;
			best_collisions = collisions;
			best_param = p;
			printf("new best candidate at %d: err %lu best %lu diff %lu\n",
			       j, collisions2, best_error, collisions2 - best_error);
			if (best_error - collisions2 == 0) {
				printf("found good enough candidate after %d\n", j);
				break;
			}
		}
	}

	for (i = 0; i < base_n; i++) {
		params[i] = best_param;
	}

	update_running_hash(ctx, params, base_n);

	for (i = base_n; i < N_PARAMS - 1; i++) {
		best_error = calc_best_error(ctx, i + 1);
		best_param = 0;
		best_collisions = ctx->n + 2;
		best_collisions2 = UINT64_MAX;
		for (j = 0; j < n_candidates; j++) {
			params[i] = rand64(ctx->rng);

			collisions = test_params_with_l2(ctx,
							 params,
							 i + 1,
							 &collisions2,
							 best_collisions2,
							 false);
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
		update_running_hash(ctx, params, i + 1);
		remove_non_colliding_strings(ctx, params, i + 1);
	}

	best_param = 0;
	/* try extra hard for the last round */
	uint64_t attempts = n_candidates * original_n_strings / ctx->n;
	printf("making %lu last round attempts\n", attempts);

	for (j = 0; j < attempts; j++) {
		params[N_PARAMS - 1] = rand64(ctx->rng);
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

	uint64_t *hits = calloc((1 << size), sizeof(uint16_t));

	struct hashcontext ctx = {data, strings.n_strings, hits};

	ctx.bits = bits;
	ctx.rng = rng;

	if (! check_raw_hash(&ctx)) {
		printf("This will never work because the raw hash collides\n");
		exit(1);
	}

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
