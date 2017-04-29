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
#define ANNEAL_CANDIDATES 3
#define TEMPERATURE 0
#define ANNEAL_ROUNDS 300
#define INIT_PARAM_CANDIDATES 30000
#define N_CANDIDATES 5

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
#define MR_SHIFT(x) (((x) >> 52ULL) & 63)
#define MR_MUL(x) ((x) & ((1ULL << 52ULL) - 1ULL))

#define MR_MASK(i) (((1 << BITS_PER_PARAM) - 1) << (i * BITS_PER_PARAM))

static inline uint32_t hash_component(uint64_t *params, uint i, uint64_t x)
{
	uint64_t param = params[i];
	uint64_t rot = MR_ROT(param);
	uint64_t mul = MR_MUL(param);
	uint64_t shift = MR_SHIFT(param);
	uint32_t mask = MR_MASK(i);
	x = ROTATE(x, rot);
	x *= mul;
	x >>= shift;
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
		printf("↻%-2lu ×%013llx »%-2lu ", MR_ROT(x), MR_MUL(x),
		       MR_SHIFT(x));
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


static uint32_t test_subset_with_alternates(struct hashcontext *ctx,
					    struct multi_rot *c, uint temp)
{
	int i, j;
	uint collisions, best_collisions;
	uint32_t *hashes = calloc(ctx->n, sizeof(uint32_t));
	uint32_t *components = calloc(ctx->n, (sizeof(uint32_t) *
					       N_PARAMS));
	uint64_t *hits = ctx->hits;
	uint best_masks[1 << N_PARAMS];
	uint n_best_masks = 0;
	uint mask, mask_change, prev_mask;
	uint32_t hash;
	uint32_t hash_mask = (1 << ctx->bits) - 1;
	struct rng *rng = ctx->rng;
	uint64_t original[N_PARAMS];
	uint64_t pool[N_PARAMS];
	memcpy(original, c->params, (N_PARAMS *
				     sizeof(c->params[0])));
	for (j = 0; j < N_PARAMS; j++) {
		if ((rand64(rng) & 3) == 0) {
			i = rand_range(rng, 0, N_PARAMS - 2);
			if (i >= j) {
				i++;
			}
			pool[j] = original[i];
		} else {
			pool[j] = rand64(rng);
		}
	}

	for (j = 0; j < ctx->n; j++) {
		hash = 0;
		uint64_t raw_hash = ctx->data[j].raw_hash;
		for (i = 0; i < N_PARAMS; i++) {
			uint32_t comp = hash_component(original, i,
						       raw_hash);
			uint32_t alt = hash_component(pool, i,
						      raw_hash);
			hash ^= comp;
			components[j * N_PARAMS + i] = comp ^ alt;
		}
		hashes[j] = hash;
	}

	best_collisions = ctx->n;
	prev_mask = 0;
	for (mask = 0; mask < (1 << N_PARAMS); mask++) {
		collisions = 0;
		for (j = 0; j < ctx->n; j++) {
			uint32_t *hcomp = components + j * N_PARAMS;
			uint32_t truncated_hash;
			hash = hashes[j];
			mask_change = mask ^ prev_mask;
			i = 0;
			while (mask_change) {
				if (mask_change & 1) {
					hash ^= hcomp[i];
				}
				mask_change >>= 1;
				i++;
			}
			truncated_hash = hash & hash_mask;
			uint64_t f = truncated_hash >> 6;
			uint64_t g = 1UL << (truncated_hash & 63);
			collisions += (hits[f] & g) ? 1 : 0;
			hits[f] |= g;
			hashes[j] = hash;
		}
		if (collisions < best_collisions) {
			best_collisions = collisions;
			best_masks[0] = mask;
			n_best_masks = 1;
		} else if (collisions == best_collisions) {
			best_masks[n_best_masks] = mask;
			n_best_masks++;
		}
		/*printf("mask %x winning %x best_collisions %d "
		       "collisions %d, hash_mask %d n_best %d\n",
		       mask, best_masks[0], best_collisions,
		       collisions, hash_mask,
		       n_best_masks);
		*/
		memset(hits, 0, (hash_mask + 1) / 8);
		prev_mask = mask;
	}

	j = 0;
	uint r = rand_range(rng, 0, n_best_masks - 1);
	uint32_t winning_mask = best_masks[r];
	uint n_changes = 0;
	while (winning_mask) {
		if (winning_mask & 1) {
			c->params[j] = pool[j];
			n_changes++;
		}
		//printf("winning mask %x n_changes %d j %d\n",
		//       winning_mask, n_changes, j);
		winning_mask >>= 1;
		j++;
	}

	c->collisions = best_collisions;
	free(hashes);
	free(components);
	ctx->stats.ignored_best_masks += n_best_masks - 1;
	return n_changes;
}

static uint test_one_subset_with_l2(struct hashcontext *ctx,
				    uint64_t *params, uint n,
				    uint *collisions2)
{
	int i, j;
	uint collisions = 0;
	uint32_t hash_mask = (1 << ctx->bits) - 1;
	uint8_t *hits = (uint8_t *) ctx->hits;
	for (j = 0; j < ctx->n; j++) {
		uint32_t hash = 0;
		uint64_t raw_hash = ctx->data[j].raw_hash;
		for (i = 0; i < n; i++) {
			uint32_t comp = hash_component(params, i,
						       raw_hash);
			hash ^= comp;
		}
		hash &= hash_mask;
		uint8_t h = hits[hash];
		if (h) {
			collisions++;
		}
		hits[hash] = MIN(h + 1, 255);
	}

	*collisions2 = 0;
	for (i = 0; i <= hash_mask; i++) {
		*collisions2 += hits[i] * hits[i];
	}

	memset(ctx->hits, 0, 1 << ctx->bits);
	return collisions;
}


static void init_multi_rot(struct hashcontext *ctx,
			   struct multi_rot *c,
			   uint64_t *params)
{
	int i, j;
	uint collisions, best_collisions = 0;
	uint collisions2, best_collisions2 = 0;

	uint64_t pool[INIT_PARAM_CANDIDATES];
	for (i = 0; i < INIT_PARAM_CANDIDATES; i++) {
		pool[i] = rand64(ctx->rng);
	}

	for (i = 0; i < N_PARAMS; i++) {
		uint64_t best_param = 0;
		bool seek_balance = (i * BITS_PER_PARAM + 8 > ctx->bits &&
				     i < N_PARAMS - 1);
		best_collisions = ctx->n + 2;
		best_collisions2 = UINT_MAX;
		for (j = 0; j < INIT_PARAM_CANDIDATES; j++) {
			params[i] = pool[j];

			if (seek_balance) {
				collisions = test_one_subset_with_l2(ctx,
								     params,
								     i + 1,
								     &collisions2);
				if (collisions2 < best_collisions2) {
					printf("collisions2 %u collisions %u\n",
					       collisions2, collisions);
					best_collisions2 = collisions2;
					best_collisions = collisions;
					best_param = params[i];
				}
			} else {
				collisions = test_one_subset(ctx,
							     params,
							     i + 1);

				if (collisions < best_collisions) {
					best_collisions = collisions;
					best_param = params[i];
				}
			}
		}
		params[i] = best_param;
	}
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

static int anneal_one_round(struct hashcontext *ctx,
			    struct multi_rot *pop,
			    uint temp)
{
	int i;
	int n_changes = 0;
	uint old_collisions;
	for (i = 0; i < N_CANDIDATES; i++) {
		struct multi_rot *c = &pop[i];
		old_collisions = c->collisions;
		n_changes += test_subset_with_alternates(ctx, c, temp);
		if (c->collisions > old_collisions) {
			printf("%d: new collisions %u  old collisions %u\n",
			       i, c->collisions, old_collisions);
			ctx->stats.worsenings ++;
		}
	}
	return n_changes;
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


static void breed_one_round(struct hashcontext *ctx,
			    struct multi_rot *pop)
{
	int i, j;
	uint n_breeders = 0;
	struct rng *rng = ctx->rng;
	struct multi_rot *a, *b, *c;

	printf("breeding..\n");

	qsort(pop, N_CANDIDATES, sizeof(struct multi_rot), cmp_multi_rot);

	n_breeders = N_CANDIDATES / 2;

	for (i = n_breeders; i < N_CANDIDATES; i++) {
		c = &pop[i];
		a = &pop[rand_range(rng, 0, n_breeders - 1)];
		b = &pop[rand_range(rng, 0, n_breeders - 1)];
		if (cmp_multi_rot(a, b) == 0) {
			if (a != b) {
				ctx->stats.inbreds++;
			}
			/* ensure non-identical pairs */
			b = c;
			/* maybe mutate one of the guilty parties */
			if (rand64(rng) & 1) {
				printf("back-breeding to increase diversity\n");
				c = a;
			}
		}

		uint64_t r = 0;
		for (j = 0; j < N_PARAMS; j++) {
			if (j % 64 == 0) {
				r = rand64(rng);
			}
			if (r & 1) {
				c->params[j] = a->params[j];
			} else {
				c->params[j] = b->params[j];
			}
			r >>= 1;
		}
		c->collisions = test_one_subset(ctx, c->params,
						N_PARAMS);
	}
	printf("killed %u; bred from %u\n", N_CANDIDATES - n_breeders,
	       n_breeders);
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


static void search_hash_space(struct hashcontext *ctx, struct multi_rot *pop)
{
	int i, j;
	uint temp = TEMPERATURE;
	const int anneals = ANNEAL_ROUNDS;
	int n_changes;

	printf("initial state \n");
	summarise_all(ctx, pop);
	describe_best(ctx, pop, false);
	exit(0);
	for (i = 0; i < 1000; i++) {
		uint total_changes = 0;
		ctx->stats.inbreds = 0;
		ctx->stats.worsenings = 0;
		ctx->stats.skipped = 0;
		ctx->stats.ignored_best_masks = 0;
		for (j = 0; j < anneals; j++) {
			n_changes = anneal_one_round(ctx, pop, temp);
			if (n_changes == 0 && j > 10) {
				printf("anneal round %d had no changes. "
				       "stopping\n", j);
				break;
			}
			total_changes += n_changes;
			if (j % 100 == 0) {
				printf("annealed %d rounds\n", j);
			}
		}

		printf("round %d; temp %u\n", i, temp);
		printf("anneal_changes %u (average %u, last %d)\n",
		       total_changes, (total_changes + (j / 2)) / j, n_changes);
		printf("skipped %u (average %u)\n",
		       ctx->stats.skipped, (ctx->stats.skipped + (j / 2)) / j);
		printf("ignored best masks %u (average %u)\n",
		       ctx->stats.ignored_best_masks,
		       (ctx->stats.ignored_best_masks + (j / 2)) / j);
		printf("worsenings %u\n", ctx->stats.worsenings / j);
		printf("inbred %u\n", ctx->stats.inbreds);

		summarise_all(ctx, pop);

		breed_one_round(ctx, pop);
		describe_best(ctx, pop, false);
		char s[20];
		sprintf(s, "multi_rot-%d.pbm", i);
		pbm_dump(pop, sizeof(pop[0]) * 8, ctx->n,
			 0, sizeof(pop[0]), s);

		temp = temp * 4 / 5;
		printf("done round %d\n", i);
	}
	printf("finishing\n");
}


static int find_hash(const char *filename, uint bits, struct rng *rng)
{
	int i;
	struct strings strings = load_strings(filename);
	struct hashdata *data = new_hashdata(&strings);
	uint size = bits + ANNEAL_CANDIDATES;

	/* sometimes hits is used as bits, needing only 1/8 the size */
	uint64_t *hits = calloc((1 << size), 1);

	struct hashcontext ctx = {data, strings.n_strings, hits};

	ctx.bits = bits;
	ctx.rng = rng;
	N_PARAMS = (bits + (BITS_PER_PARAM - 1)) / BITS_PER_PARAM;

	uint64_t *mem = calloc(N_CANDIDATES * N_PARAMS, sizeof(uint64_t));
	struct multi_rot *pop = calloc(N_CANDIDATES, sizeof(struct multi_rot));
	for (i = 0; i < N_CANDIDATES; i++) {
		init_multi_rot(&ctx, &pop[i], mem + i * N_PARAMS);
		if (i % 100 == 0) {
			printf("initialised %d/%d\n", i, N_CANDIDATES);
		}
	}

	search_hash_space(&ctx, pop);
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
