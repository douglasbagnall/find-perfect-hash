//gcc -Wall -O3 find-perfect-hash.c -o find-perfect-hash -ggdb -ffast-math -lm -march=native

#define DETERMINISTIC 0
#define HASH_COMPONENTS 8
#define ANNEAL_CANDIDATES 3
#define TEMPERATURE 0
#define ANNEAL_ROUNDS 200

#include "find-perfect-hash-helpers.h"

struct hashdata {
	const char *string;
	unsigned int stringlen;
	uint32_t raw_hash[2];
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
	uint32_t params[HASH_COMPONENTS];
	uint collisions;
};

const char *HASH_NAMES[2] = {"djb", "fnv"};

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
	hash->raw_hash[0] = h;
	hash->raw_hash[1] = h2;
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


#define MR_ROT(x) ((x) >> 27)
#define MR_HASH(x) (((x) >> 26) & 1)
#define MR_MUL(x) ((x) & 0x3ffffff)

static inline uint32_t hash_component(uint32_t param, uint32_t raw[2])
{
	uint32_t comp = raw[MR_HASH(param)];
	uint32_t rot = MR_ROT(param);
	uint32_t mul = MR_MUL(param);
	comp *= mul;
	comp = ROTATE(comp, rot);
	//comp >>= rot;
	return comp;
}

int cmp_uint32(const void *a, const void *b)
{
	return *(uint32_t *)a - *(uint32_t *)b;
}

static void describe_hash(struct hashcontext *ctx,
			  struct multi_rot *c)
{
	int i;
	uint32_t prev = 0;
	int bright = (c->collisions < 80) ? 1 : 0;
	qsort(c->params, HASH_COMPONENTS, sizeof(uint32_t), cmp_uint32);
	if (c->collisions == 0) {
		printf("\033[01;31m");
	};
	printf("collisions %-3u ", c->collisions);
	printf("\033[00mbits: ");
	for (i = 0; i < HASH_COMPONENTS; i++) {
		uint32_t x = c->params[i];
		if (i) {
			printf("\033[01;34m ^ ");
		}
		if (x == prev) {
			printf("\033[01;33m");
		} else {
			printf("\033[0%d;%dm", bright, 36 + (i & 1));
		}
		printf("%s ", MR_HASH(x) ? "fnv": "djb");
		printf("↻%-2x ×%07x ", MR_ROT(x), MR_MUL(x));
		prev = x;
	}
	printf("\033[00m\n");
}

static uint32_t test_one_subset(struct hashcontext *ctx,
				struct multi_rot *c)
{
	int i, j;
	uint collisions = 0;
	uint32_t hash_mask = (1 << ctx->bits) - 1;
	for (j = 0; j < ctx->n; j++) {
		uint32_t hash = 0;
		uint32_t *raw_hash = ctx->data[j].raw_hash;
		for (i = 0; i < HASH_COMPONENTS; i++) {
			uint32_t x = c->params[i];
			uint32_t comp = hash_component(x, raw_hash);
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

static void shuffle_uint32(struct rng *rng, uint32_t *array, uint n)
{
	uint i, a;
	for (i = 0; i < n - 1; i++) {
		a = rand_range(rng, i, n - 1);
		uint32_t tmp = array[i];
		array[i] = array[a];
		array[a] = tmp;
	}
}


static int count_sorted_differences(uint32_t *a, uint32_t *b, uint n)
{
	/* we assume the lists are both sorted */
	int count = n;
	int i = 0, j = 0;
	while (true) {
		//printf("n %u a[%d] %u b[%d] %u\n",
		//       n, i, a[i], j, b[j]);
		if (a[i] > b[j]) {
			j++;
			if (j == n) {
				break;
			}
			continue;
		}
		if (a[i] < b[j]) {
			i++;
			if (i == n) {
				break;
			}
			continue;
		}
		/* a[i] == b[j] */
		count--;
		if (j == n - 1 && i == n - 1) {
			break;
		}

		if (i < n - 1) {
			i++;
		}
		if (j < n - 1) {
			j++;
		}
	}
	return count;
}


static uint32_t test_subset_with_dropout(struct hashcontext *ctx,
					 struct multi_rot *c,
					 uint dropout, uint temp)
{
	int i, j;
	uint collisions, best_collisions;
	const uint size = HASH_COMPONENTS + dropout;
	uint32_t *hashes = calloc(ctx->n, sizeof(uint32_t));
	uint32_t *components = calloc(ctx->n, sizeof(uint32_t) * size);

	uint64_t *hits = ctx->hits;
	uint32_t best_mask = 0;
	uint32_t ignored_best_masks = 0;
	uint32_t mask, hash, n_changes, mask_change, prev_mask;
	uint32_t hash_mask = (1 << ctx->bits) - 1;
	struct rng *rng = ctx->rng;

	uint32_t original[HASH_COMPONENTS];
	uint32_t pool[size];
	memcpy(pool, c->params, sizeof(c->params));
	memcpy(original, c->params, sizeof(c->params));

	for (j = HASH_COMPONENTS; j < size; j++) {
		pool[j] = rand64(rng);
	}

	shuffle_uint32(rng, pool, size);

	for (j = 0; j < ctx->n; j++) {
		hash = 0;
		uint32_t *raw_hash = ctx->data[j].raw_hash;
		for (i = 0; i < size; i++) {
			uint32_t x = pool[i];
			uint32_t comp = hash_component(x, raw_hash);
			hash ^= comp;
			components[j * size + i] = comp;
		}
		hashes[j] = hash;
	}

	best_mask = 0;
	best_collisions = ctx->n;
	mask = (1 << dropout) - 1;
	prev_mask = 0;
	while (mask < (1 << size)) {
		collisions = 0;
		for (j = 0; j < ctx->n; j++) {
			uint32_t *hcomp = components + j * size;
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
			//printf("f %lu g %lx 1<< bits / 8 %u\n",
			//       f, g, (1 << ctx->bits) / 8);
			collisions += (hits[f] & g) ? 1 : 0;
			hits[f] |= g;
			hashes[j] = hash;
		}
		if (collisions < best_collisions) {
			best_collisions = collisions;
			best_mask = mask;
			ignored_best_masks = 0;
		} else if (collisions == best_collisions) {
			ignored_best_masks++;
		}

		memset(hits, 0, (1 << ctx->bits) / 8);
		prev_mask = mask;
		uint32_t t = (mask | (mask - 1)) + 1;
		mask = t | ((((t & -t) / (mask & -mask)) >> 1) - 1);
	}

	j = 0;
	//printf("best mask %x\n", best_mask);
	uint32_t mask_mask = best_mask;
	for (i = 0; i < size; i++, mask_mask >>= 1) {
		if (mask_mask & 1) {
			continue;
		}
		c->params[j] = pool[i];
		j++;
	}

	qsort(c->params, HASH_COMPONENTS, sizeof(uint32_t), cmp_uint32);
	n_changes = count_sorted_differences(original, c->params, HASH_COMPONENTS);

	c->collisions = test_one_subset(ctx, c);
	if (c->collisions != best_collisions)  {
		printf("best collisions %u, test_one_subset %u\n",
		       best_collisions, c->collisions);
	}
	
	free(hashes);
	free(components);
	ctx->stats.ignored_best_masks += ignored_best_masks;
	return n_changes;
}


static void init_multi_rot(struct hashcontext *ctx,
			   struct multi_rot *c)
{
	int i;
	struct rng *rng = ctx->rng;

	for (i = 0; i < HASH_COMPONENTS; i++) {
		c->params[i] = rand64(rng);
	}
	qsort(c->params, HASH_COMPONENTS, sizeof(uint32_t), cmp_uint32);
	c->collisions = test_one_subset(ctx, c);
}

#define N_CANDIDATES 1000

static void summarise_all(struct multi_rot *pop)
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
		n_changes += test_subset_with_dropout(ctx, c, ANNEAL_CANDIDATES, temp);
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
	return memcmp(a->params, b->params, HASH_COMPONENTS);
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

		uint64_t r;
		for (j = 0; j < HASH_COMPONENTS; j++) {
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
		qsort(c->params, HASH_COMPONENTS, sizeof(uint32_t), cmp_uint32);
		c->collisions = test_one_subset(ctx, c);
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
	summarise_all(pop);
	describe_best(ctx, pop, false);

	for (i = 0; i < 1000; i++) {
		uint total_changes = 0;
		ctx->stats.inbreds = 0;
		ctx->stats.worsenings = 0;
		ctx->stats.skipped = 0;
		ctx->stats.ignored_best_masks = 0;
		for (j = 0; j < anneals; j++) {
			n_changes = anneal_one_round(ctx, pop, temp);
			if (n_changes == 0) {
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

		summarise_all(pop);

		breed_one_round(ctx, pop);
		/*
		for (j = 0; j < N_CANDIDATES; j++) {
			struct multi_rot *c = &pop[j];
			c->collisions = test_one_subset(ctx, c);
		}
		*/
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

	uint64_t *hits = calloc((1 << size) / 64, sizeof(uint64_t));

	struct hashcontext ctx = {data, strings.n_strings, hits};

	ctx.bits = bits;
	ctx.rng = rng;

	struct multi_rot *pop = calloc(N_CANDIDATES, sizeof(struct multi_rot));
	for (i = 0; i < N_CANDIDATES; i++) {
		init_multi_rot(&ctx, &pop[i]);
	}

	search_hash_space(&ctx, pop);
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
	struct rng rng;
#if DETERMINISTIC
	rng_init(&rng, 12345);
#else
	rng_random_init(&rng);
#endif
	return find_hash(argv[1], bits, &rng);
}
