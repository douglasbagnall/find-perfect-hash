//gcc -Wall -O3 find-perfect-hash.c -o find-perfect-hash -ggdb -ffast-math -lm -march=native

#define BITS_PER_PARAM 1
#define BASE_N 4
#define DETERMINISTIC 0
#define MAX_SMALL_TUPLE 20

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

struct hash_big_tuple {
	uint64_t array[MAX_SMALL_TUPLE];
	uint n;
};

struct tuple_list {
	uint64_t *raw;
	uint tuple_size;
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


static void find_unresolved_small_tuples(struct hashcontext *ctx,
					 uint64_t *params, uint n,
					 struct hash_tuples *dest,
					 uint max_size)
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

	for (j = 0; j < ctx->n; j++) {
		uint32_t raw = ctx->data[j].raw_hash;
		uint32_t hash = unmasked_hash(raw, params, n);
		hash &= hash_mask;
		struct hash_big_tuple *p = &tuples[hash];
		if (p->n < max_size) {
			p->array[p->n] = raw;
		} else {
			printf("\033[01;31mmore than %u collisions for hash %x"
			       "\033[00m\n\n", p->n, hash);
		}
		p->n++;
	}

	for (j = 0; j < n_hashes; j++) {
		uint count = MIN(tuples[j].n, max_size + 1);
		size_counts[count]++;
	}
	printf("%u empty buckets\n", size_counts[0]);
	printf("%u singletons\n", size_counts[1]);
	for (j = 2; j <= max_size; j++) {
		printf("%u tuples of size %u\n", size_counts[j], j);
	}
	printf("%u tuples of size > %u\n", size_counts[max_size + 1],
	       max_size);

	dest->tuples[0] = (struct tuple_list){NULL, 0, 0};
	dest->tuples[1] = (struct tuple_list){NULL, 1, 0};

	for (j = 0; j <= max_size; j++) {
		uint64_t *array = calloc(j * size_counts[j], sizeof(array[0]));
		dest->tuples[j] = (struct tuple_list){array, j, 0};
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
		if (size_counts[j] != dest->tuples[j].n) {
			printf("expected %u %u-tuples, got %u\n",
			       size_counts[j], j, dest->tuples[j].n);
		}
	}
	free(tuples);
	free(size_counts);
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


static uint do_squashing_round(struct hashcontext *ctx,
			       struct multi_rot *c,
			       uint n_candidates,
			       uint64_t attempts,
			       uint n,
	bool relax)
{
	uint i, k;
	uint64_t j;
	uint collisions, best_collisions = UINT_MAX;
	uint64_t best_param = 0;
	uint64_t *params = c->params;
	struct hash_tuples tuples;
	uint n_bits = n + BASE_N - 1;
	uint64_t past_triples = 0;
	find_unresolved_small_tuples(ctx, params, n,
				     &tuples, 4);

	attempts *= 10;
	printf("making %lu attempts\n", attempts);

	START_TIMER(squashing);

	//printf("non-collisions mask %lx\n", ~((1UL << n_bits) - 1));

	for (j = 0; j < attempts; j++) {
		/* we don't need to calculate the full hash */
		uint64_t param = next_param(ctx->rng, j, params, n + 1);
		/* we don't care about non-collisions to the right of this parameter */
		uint64_t non_collisions = ~((1UL << n_bits) - 1);
		uint64_t mul = MR_MUL(param);
		param &= ~MR_ROT_MASK;
		uint64_t a, b, c, d;
		uint64_t ab, ac, ad, bd, cd, abcd;
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
			//bc = b ^ c;
			bd = b ^ d;
			cd = c ^ d;
			abcd = ((~ab & ~ac) &
				(~bd & ~cd) &
				(~ad & ~bd) &
				(~ac & ~cd)); /* 1 bits mean collisions */
			non_collisions &= ~abcd;
		}
		if (non_collisions == 0) {
			/* no rotate of this mul works, try with another */
			continue;
		}
		if (! relax) {
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
		}
		past_triples++;
		//printf("got past triples at %lu. non-collisions %016lx\n", j, non_collisions);
		/* at this point we have met the mandatory criteria, and
		   increased the number of pairs for the next round by
		   2 * n_quads + 1 * n_triples.

		   we want to reduce the number of pairs in this round as much
		   as possible, so check the good rotates found in the
		   quad/triple check for pair elimination */

		non_collisions >>= n_bits;

		for (k = 0; non_collisions; k++) {
			if (non_collisions & 1) {
				uint64_t p = param + k * MR_ROT_STEP;
				collisions = test_all_pairs(p,
							    tuples.tuples[2],
							    n + 1);
				if (collisions < best_collisions) {
					best_collisions = collisions;
					best_param = p;
					printf("new squashing best %15lx »%-2lu at %lu:"
					       " collisions %u\n",
					       MR_MUL(p), MR_ROT(p), j, collisions);
					if (collisions == 0) {
						goto win;
					}
				}
			}
			non_collisions >>= 1;
		}
	}
  win:
	printf("past triples %lu times\n", past_triples);
	params[n] = best_param;
	PRINT_TIMER(squashing);
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
			  uint n_candidates,
			  uint64_t attempts)
{
	uint i;
	uint64_t j;
	uint collisions, best_collisions = UINT_MAX;
	uint64_t best_param = 0;
	uint64_t *params = c->params;
	struct hash_tuples tuples;

	find_unresolved_small_tuples(ctx, params, N_PARAMS - 1,
				     &tuples, 2);

	struct tuple_list pairs = tuples.tuples[2];

	printf("There are %u unresolved pairs\n", pairs.n);
	attempts *= 10;

	START_TIMER(last);

	if (pairs.n < 64 &&
	    (1UL << pairs.n) < attempts * 6400UL) {
		uint64_t exact_attempts = attempts * 100;
		uint best_run = 0;
		printf("trying for exact solution with %lu attempts (64 way parallel)\n",
		       exact_attempts);

		/* Test all rotations at once:

		   XOR the two multiplies.
		   anti-collisions are 1s.
		   AND the anti-collisions of pairs.
		   if there is a one bit, that rotation is good.
		 */
		for (j = 0; j < exact_attempts; j++) {
			/* we don't need to calculate the full hash */
			uint64_t p = next_param(ctx->rng, j, params, N_PARAMS);
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
					       "at %lu: %u pairs\n",
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
						printf("WINNING run %15lx »%-2lu at %lu\n",
						       MR_MUL(p), MR_ROT(p), j);
						goto win;
					}
					p += MR_ROT_STEP;
				}
				printf("we thought we has a winning param, but no!"
				       "%15lx »?? at %lu\n",
				       MR_MUL(p), j);
			}
		}
	}
	printf("trying for an inexact solution with %lu attempts\n",
	       attempts);

	for (j = 0; j < attempts; j++) {
		/* we don't need to calculate the full hash */
		uint64_t p = next_param(ctx->rng, j, params, N_PARAMS);
		collisions = test_all_pairs(p, pairs, N_PARAMS);
		if (collisions < best_collisions) {
			best_collisions = collisions;
			best_param = p;
			printf("new final round best %15lx »%-2lu at %lu: collisions %u\n",
			       MR_MUL(p), MR_ROT(p), j, collisions);
			if (collisions == 0) {
				printf("found a winner after %lu\n", j);
				break;
			}
		}
	}
  win:
	params[N_PARAMS - 1] = best_param;
	PRINT_TIMER(last);
	best_collisions = test_params_running(ctx, params, N_PARAMS,
					      best_collisions);

	free_tuple_data(&tuples);
	return best_collisions;

}


static void do_l2_round(struct hashcontext *ctx,
			struct multi_rot *c,
			uint n_candidates,
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
	printf("making %lu attempts\n", attempts);

	for (j = 0; j < attempts; j++) {
		params[n] = next_param(ctx->rng, j, params, n);
		collisions2 = test_params_with_l2_running(
			ctx,
			params,
			n + 1);
		if (collisions2 < best_collisions2) {
			best_collisions2 = collisions2;
			best_param = params[n];
			printf("new best %15lx »%-2lu at %lu: "
			       "err %lu > %lu; diff %lu\n",
			       MR_MUL(best_param), MR_ROT(best_param),
			       j, collisions2, best_error,
			       collisions2 - best_error);
			if (collisions2 == best_error) {
				printf("found good candidate after %lu\n", j);
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
	update_running_hash(ctx, params, n + 1);
	PRINT_TIMER(l2);
}

static void init_multi_rot(struct hashcontext *ctx,
			   struct multi_rot *c,
			   uint n_candidates)
{
	uint i;
	uint worst = ctx->n;
	uint64_t attempts;
	uint64_t original_n_strings = ctx->n;
	uint64_t *params = c->params;
	update_running_hash(ctx, params, 0);

	for (i = 0; i < N_PARAMS - 2; i++) {
		attempts = (uint64_t)n_candidates * original_n_strings / ctx->n;
		if (worst > 4 || true) {
			do_l2_round(ctx, c, n_candidates, attempts, i);
		} else {
			printf("small tuple squashing\n");
			do_squashing_round(ctx, c, n_candidates, attempts, i, true);
		}			
		worst = remove_non_colliding_strings(ctx, params, i + 1);
	}

	/* try extra hard for the last two rounds */
	if (worst > 4) {
		printf("the situation is hopeless. stopping\n");
		return;
	}
	attempts = (uint64_t)n_candidates * original_n_strings / ctx->n;
	do_squashing_round(ctx, c, n_candidates, attempts, N_PARAMS - 2, false);
	c->collisions = do_last_round(ctx, c, n_candidates, attempts);
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
	c.collisions = UINT_MAX;
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
