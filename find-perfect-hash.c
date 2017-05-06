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
#define INIT_PARAM_CANDIDATES 100000
#define N_CANDIDATES 1

#include "find-perfect-hash-helpers.h"

struct hashdata {
	const char *string;
	unsigned int stringlen;
	uint64_t raw_hash;
};

struct stats {
	uint worsenings;
	uint inbreds;
	uint skipped;
	uint ignored_best_masks;
};

struct hashcontext {
	struct hashdata *data;
	uint n;
	uint64_t *hits;
	struct stats stats;
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
}

struct hashdata *new_hashdata(struct strings *strings)
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


int cmp_uint64(const void *a, const void *b)
{
	return *(uint64_t *)a - *(uint64_t *)b;
}

static void describe_hash(struct hashcontext *ctx,
			  struct multi_rot *c)
{
	int i;
	uint64_t prev = 0;
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
		if (x == prev) {
			printf("\033[01;33m");
		} else {
			printf("\033[0%d;%dm", bright, 36 + (i & 1));
		}
		printf("↻%-2lu ×%013llx ", MR_ROT(x), MR_MUL(x));
		printf("& %04x ", MR_MASK(i));
		prev = x;
	}
	printf("\033[00m\n");
}

static uint test_one_subset(struct hashcontext *ctx,
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



static uint test_one_subset_with_l2(struct hashcontext *ctx,
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
		hits[hash] = MIN(h + 1, 255);
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

static uint64_t calc_best_error(struct hashcontext *ctx, uint n_bits)
{
	uint n = 1 << n_bits;
	uint q = ctx->n / n;
	uint r = ctx->n % n;
	uint64_t sum = n * q * q + 2 * q * r + r;
	return sum;
}

static void init_multi_rot(struct hashcontext *ctx,
			   struct multi_rot *c,
			   uint64_t *params)
{
	int i, j;
	uint collisions, best_collisions;
	uint64_t collisions2, best_collisions2 = 0;
	uint64_t best_param = 0;
	uint64_t best_error;
	uint64_t *pool = calloc(INIT_PARAM_CANDIDATES, sizeof(uint64_t));
	for (i = 0; i < INIT_PARAM_CANDIDATES; i++) {
		pool[i] = rand64(ctx->rng);
	}

	best_collisions = ctx->n + 2;
	best_collisions2 = UINT64_MAX;

	const int base_n = MIN(6, N_PARAMS * 2 / 3);

	best_error = calc_best_error(ctx, base_n);
	for (j = 0; j < INIT_PARAM_CANDIDATES; j++) {
		uint64_t p = pool[j];
		for (i = 0; i < base_n; i++) {
			params[i] = p;
		}
		collisions = test_one_subset_with_l2(ctx,
						     params,
						     base_n,
						     &collisions2);
		if (collisions2 < best_collisions2) {
			best_collisions2 = collisions2;
			best_collisions = collisions;
			best_param = p;
			printf("new best candidate at %d: err %lu > %lu diff %lu\n",
			       j, collisions2, best_error, collisions2 - best_error);
			if (collisions2 < best_error + (best_error >> 7)) {
				printf("found good enough candidate after %d\n", j);
			}
		}
	}

	for (i = 0; i < base_n; i++) {
		params[i] = best_param;
	}
	printf("base collisions %u, squared %lu, ratio %.3f\n",
	       best_collisions, best_collisions2,
	       best_collisions2 * (1 << base_n) * 1.0 / (ctx->n * ctx->n));

	for (i = base_n; i < N_PARAMS - 1; i++) {
		best_param = 0;
		best_collisions = ctx->n + 2;
		best_collisions2 = UINT64_MAX;
		for (j = 0; j < INIT_PARAM_CANDIDATES; j++) {
			params[i] = pool[j];

			collisions = test_one_subset_with_l2(ctx,
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
				if (collisions2 < best_error + (best_error >> 7)) {
					printf("found good enough candidate after %d\n", j);
				}
			}
		}
		params[i] = best_param;
	}

	best_param = 0;

	for (j = 0; j < INIT_PARAM_CANDIDATES; j++) {
		params[N_PARAMS - 1] = pool[j];
		collisions = test_one_subset(ctx,
					     params,
					     N_PARAMS);
		if (collisions < best_collisions) {
			best_collisions = collisions;
			best_param = pool[j];
		}
	}
	free(pool);
	c->params = params;
	c->collisions = best_collisions;
}


static void summarise_all(struct hashcontext *ctx,
			  struct multi_rot *pop)
{
	uint32_t c;
	uint min_collisions = UINT_MAX;
	uint max_collisions = 0;
	uint sum = 0;
	uint sum2 = 0;
	uint mean, variance, stddev;
	uint range_top;
	uint range, bucket_max, step;
	uint *counts;
	int n_buckets;
	int i, j;
	for (i = 0; i < N_CANDIDATES; i++) {
		c = pop[i].collisions;
		max_collisions = MAX(c, max_collisions);
		min_collisions = MIN(c, min_collisions);
		sum += c;
		sum2 += c * c;
	}
	mean = (sum + N_CANDIDATES / 2) / N_CANDIDATES;
	variance = ((sum2 - sum * sum  + N_CANDIDATES / 2) /
		    (N_CANDIDATES * N_CANDIDATES));
	stddev = sqrtf(variance) + 0.5;

	printf(" mean %u stddev %u\n", mean, stddev);

	range_top = MIN(max_collisions, mean + 4 * stddev);
	range = range_top - min_collisions;

	step = MAX(range / 40, 1);
	n_buckets = range / step + 1;
	counts = calloc(n_buckets + 1, sizeof(counts[0]));

	printf("range_top %u\n", range_top);
	j = 0;
	for (i = 0; i < N_CANDIDATES; i++) {
		c = pop[i].collisions;
		j = (c - min_collisions) / step;
		if (j >= n_buckets) {
			j = n_buckets - 1;
		}
		counts[j]++;
	}
	bucket_max = 0;
	for (i = 0; i < n_buckets; i++) {
		bucket_max = MAX(bucket_max, counts[i]);
	}
	printf("buckets size %u\n", step);
	for (i = 0; i < n_buckets; i++) {
		if (counts[i] == 0) {
			printf("\033[00;37m");
		}
		uint centre = min_collisions + i * step + step / 2;
		if (i == n_buckets - 1 && range_top != max_collisions) {
			printf("%3u+ %5u \033[00;33m", centre, counts[i]);
		} else {
			printf("%4u %5u \033[00;33m", centre, counts[i]);
		}
		if (bucket_max > 80) {
			for (j = 0; j < (counts[i] * 80 + 40) / bucket_max; j++) {
				putchar('*');
			}
		} else {
			for (j = 0; j < counts[i]; j++) {
				putchar('*');
			}
		}

		puts("\033[00m");
	}
	free(counts);
}


int cmp_multi_rot(const void *va, const void *vb)
{
	struct multi_rot *a = (struct multi_rot *)va;
	struct multi_rot *b = (struct multi_rot *)vb;
	if (a == b) {
		return 0;
	}
	int c = (int)a->collisions - (int)b->collisions;
	if (c != 0) {
		return c;
	}
	return memcmp(a->params, b->params, N_PARAMS * sizeof(uint64_t));
}



static void describe_best(struct hashcontext *ctx,
			  struct multi_rot *pop, bool already_sorted)
{
	int i;
	int count, cmp;
	struct multi_rot *c, *d;

	if (! already_sorted) {
		qsort(pop, N_CANDIDATES, sizeof(struct multi_rot),
		      cmp_multi_rot);
	}

	c = pop;
	count = 1;
	for (i = 1; i < N_CANDIDATES; i++) {
		d = &pop[i];
		cmp = cmp_multi_rot(c, d);
		if (cmp == 0) {
			count++;
			continue;
		}
		if (cmp > 0) {
			printf("list seems unsorted!\n");
			return;
		}
		printf("%3d× ", count);
		describe_hash(ctx, c);
		if (c->collisions < d->collisions) {
			return;
		}
		count = 1;
		c = d;
	}
}


static int find_hash(const char *filename, uint bits, struct rng *rng)
{
	int i;
	struct strings strings = load_strings(filename);
	struct hashdata *data = new_hashdata(&strings);
	N_PARAMS = (bits + (BITS_PER_PARAM - 1)) / BITS_PER_PARAM;
	uint size = N_PARAMS * BITS_PER_PARAM;

	uint64_t *hits = calloc((1 << size), 2);

	struct hashcontext ctx = {data, strings.n_strings, hits};

	ctx.bits = bits;
	ctx.rng = rng;

	uint64_t *mem = calloc(N_CANDIDATES * N_PARAMS, sizeof(uint64_t));
	struct multi_rot *pop = calloc(N_CANDIDATES, sizeof(struct multi_rot));
	for (i = 0; i < N_CANDIDATES; i++) {
		init_multi_rot(&ctx, &pop[i], mem + i * N_PARAMS);
		if (i % 100 == 0) {
			printf("initialised %d/%d\n", i, N_CANDIDATES);
		}
	}

        summarise_all(&ctx, pop);
        describe_best(&ctx, pop, false);

	free(hits);
	free(pop);
	free(mem);
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
	struct rng rng;
#if DETERMINISTIC
	rng_init(&rng, 12345);
#else
	rng_random_init(&rng);
#endif
	return find_hash(argv[1], bits, &rng);
}
