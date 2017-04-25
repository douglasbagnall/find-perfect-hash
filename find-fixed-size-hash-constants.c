#include <inttypes.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

//gcc -Wall -O3 find-fixed-size-hash-constants.c -o find-fixed-size-hash-constants -ggdb -ffast-math

#define DEBUG(format, ...) do {                                 \
    fprintf (stderr, (format),## __VA_ARGS__);                  \
    fputc('\n', stderr);                                         \
    fflush(stderr);                                             \
    } while (0)

#ifndef MAX
#define MAX(a, b)  (((a) >= (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#endif

#define POP 4000
#define STAT_P (4 * 1024)
#define STAT_MEAN(x) (((x) + STAT_P / 2) / STAT_P)

struct stats {
	unsigned int victims, mutations, inbreds;
};


enum hash_param {
	CONTROL = 0,
	H1_INIT,
	H2_INIT,
	SHIFT_A,
	SHIFT_B,
	SHIFT_C,  /* 5 */
	SHIFT_D,
	SHIFT_E,
	SHIFT_F,
	MOD_1,
	MOD_2,    /* 10 */
	MOD_LATE,
	XOR,
	ADD,
	ADD_1_LATE,
	ADD_2_LATE, /* 15 */
	XORSHIFT_1_LATE,
	XORSHIFT_2_LATE,
	XORSHIFT_3_LATE,
	PARAM_LAST
};

const uint32_t param_width[PARAM_LAST] = {
	PARAM_LAST,
	31,
	31,
	5,    /* A */
	5,
	31,   /* C */
	5,
	5,
	31,   /* F */
	31,   /* mod 1*/
	31,
	31,
	4,    /* xor */
	4,    /* add */
	15,
	15,   /*add 2 late */
	5,    /*xorshift 1 late*/
	5,
	5,
};

const uint32_t param_odd[PARAM_LAST] = {
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	1,   /* mods are always odd */
	1,
	1,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};

struct hash {
	uint32_t params[PARAM_LAST];
	char *blameworthy_string_1;
	char *blameworthy_string_2;
	int score;
};

struct rng {
	uint64_t a;
	uint64_t b;
	uint64_t c;
	uint64_t d;
};

#define ROTATE(x, k) (((x) << (k)) | ((x) >> (sizeof(x) * 8 - (k))))

static uint64_t rand64(struct rng *x)
{
	uint64_t e = x->a - ROTATE(x->b, 7);
	x->a = x->b ^ ROTATE(x->c, 13);
	x->b = x->c + ROTATE(x->d, 37);
	x->c = x->d + e;
	x->d = e + x->a;
	return x->d;
}

static void rng_init(struct rng *x, uint64_t seed)
{
	int i;
	x->a = 0xf1ea5eed;
	x->b = x->c = x->d = seed;
	for (i = 0; i < 20; ++i) {
		(void)rand64(x);
	}
}

static void rng_random_init(struct rng *rng)
{
	/* random enough for this */
	struct timespec t;
	uint64_t seed;
	clock_gettime(CLOCK_REALTIME, &t);
	seed = ((uint64_t)t.tv_nsec << 20) + t.tv_sec;
	seed ^= (uintptr_t)rng;
	seed += (uintptr_t)&MIN_A;
	printf("seeding with %lu\n", seed);
	rng_init(rng, seed);
}


//#define FNV 1099511628211UL
#define FNV 16777619

static inline void internal_hash(uint32_t *h, uint32_t *h2, const char *s)
{
	while (*s != '\0') {
		uint8_t c = (uint8_t)*s & 0x5f;
		*h2 ^= c;
		*h2 *= FNV;
                *h = ((*h << 5) + *h) ^ c;
		s++;
	}
}

#define GET_VAL(i) ((control & (1 << ((i) - 1))) ? hash->params[i] : 0)

static uint32_t casefold_hash(struct hash *hash, const char *s)
{
	uint32_t h, h3;
	uint32_t control = hash->params[CONTROL];
	//printf("H2_INIT %x params[H2_INIT] %x GETVAL %x CONTROLS & 3 %x\n",
	//       H2_INIT, hash->params[H2_INIT], GET_VAL(H2_INIT), control &3);

	uint32_t h1 = GET_VAL(H1_INIT);
	uint32_t h2 = GET_VAL(H2_INIT);

	uint32_t A = GET_VAL(SHIFT_A);
	uint32_t B = GET_VAL(SHIFT_B);
	uint32_t C = GET_VAL(SHIFT_C);
	uint32_t D = GET_VAL(SHIFT_D);
	uint32_t E = GET_VAL(SHIFT_E);
	uint32_t F = GET_VAL(SHIFT_F);
	uint32_t mod1 = GET_VAL(MOD_1);
	uint32_t mod2 = GET_VAL(MOD_2);
	uint32_t mod_late = GET_VAL(MOD_LATE);
	uint32_t xor = GET_VAL(XOR);
	uint32_t add = GET_VAL(ADD);
	uint32_t add_1_late = GET_VAL(ADD_1_LATE);
	uint32_t add_2_late = GET_VAL(ADD_2_LATE);
	uint32_t xorshift_1_late = GET_VAL(XORSHIFT_1_LATE);
	uint32_t xorshift_2_late = GET_VAL(XORSHIFT_2_LATE);
	uint32_t xorshift_3_late = GET_VAL(XORSHIFT_3_LATE);

	internal_hash(&h1, &h2, s);
	if (mod1) {
		h1 %= mod1;
	}
	if (mod2) {
		h2 %= mod2;
	}

	h = h1;
	if (add) {
		h += h2 + add;
	}
	if (xor) {
		h ^= h2 + xor;
	}
	if (A) {
		h ^= h >> A;
	}
	if (B) {
		h ^= (h1 >> B) + C;
	}
	if (D) {
		h ^= (h2 >> D);
	}
	if (E) {
		h ^= (h2 >> E) + F;
	}
	h3 = h;
	if (mod_late) {
		h %= mod_late;
	}
	if (add_1_late) {
		h += h1 + add_1_late;
	}
	if (add_2_late) {
		h += h2 + add_2_late;
	}
	if (xorshift_1_late) {
		h ^= (h1 >> xorshift_1_late);
	}
	if (xorshift_2_late) {
		h ^= (h2 >> xorshift_2_late);
	}
	if (xorshift_3_late) {
		h ^= (h3 >> xorshift_3_late);
	}
	return h;
}

struct strings {
	int n_strings;
	char *mem;
	char **strings;
};

static struct strings load_strings(const char *filename)
{
	int i, j;
	int len;
	size_t read;

	struct strings s = {0, 0, 0};

	FILE *f = fopen(filename, "r");
	if (f == NULL) {
		printf("could not open %s\n", filename);
		return s;
	}
	fseek(f, 0, SEEK_END);
	len = ftell(f);
	rewind(f);

	s.mem = malloc(len + 1);
	if (s.mem == NULL) {
		printf("could not allocate %d bytes?!\n", len + 1);
		return s;
	}
	read = fread(s.mem, 1, len, f);
	if (read != len) {
		printf("gah, C file handling, wanted %d, got %lu\n",
		       len, read);
		free(s.mem);
		return s;
	}
	for (i = 0; i < len; i++) {
		if (s.mem[i] == '\n') {
			s.mem[i] = '\0';
			s.n_strings++;
		}
	}
	s.strings = malloc(len * sizeof(char*));

	s.strings[0] = s.mem;
	j = 1;
	for (i = 0; i < len; i++) {
		if (s.mem[i] == '\0' && j < s.n_strings) {
			s.strings[j] = s.mem + i + 1;
			j++;
		}
	}
	return s;
}

static inline uint32_t rand_range(struct rng *rng, uint32_t low, uint32_t high)
{
	uint64_t r = rand64(rng);
	return low + r % (high - low + 1);
}


static inline uint32_t rand_range_excl(struct rng *rng,
				       uint32_t low, uint32_t high, uint32_t exc)
{
	uint32_t x;
	do {
		x = rand_range(rng, low, high);
	} while (x == exc);
	return x;
}


static void shuffle_strings(struct strings *s, struct rng *rng)
{
	int i, a;
	char *tmp;
	for (i = 0; i < s->n_strings - 1; i++) {
		a = rand_range(rng, i, s->n_strings - 1);
		tmp = s->strings[i];
		s->strings[i] = s->strings[a];
		s->strings[a] = tmp;
	}
}

static void init_hash(struct hash *hash, struct rng *rng, uint32_t mask)
{
	hash->score = 0;
	hash->blameworthy_string_1 = NULL;
	hash->blameworthy_string_2 = NULL;

	for (int i = 0; i < PARAM_LAST; i++) {
		uint32_t mask = (1 << param_width[i]) - 1;
		hash->params[i] = rand64(rng) & mask;
		hash->params[i] |= param_odd[i];
	}
}

static uint32_t test_one_hash(struct hash *hash, struct strings *s,
			      struct rng *rng, uint32_t mask,
			      struct stats *stats)
{
	int i;
	uint32_t h = 0;
	uint32_t score = 0;
	int hits[mask + 1];
	if (hash->score) {
		return hash->score;
	}
	hash->blameworthy_string_1 = NULL;
	hash->blameworthy_string_2 = NULL;
	memset(hits, 0xff, (mask + 1) * sizeof(hits[0]));
	for (i = 0; i < s->n_strings; i++) {
		uint32_t raw = casefold_hash(hash, s->strings[i]);
		h = raw & mask;
		if (hits[h] >= 0) {
			if (hash->blameworthy_string_1 == NULL) {
				hash->blameworthy_string_1 = s->strings[hits[h]];
				hash->blameworthy_string_2 = s->strings[i];
			}
		} else {
			hits[h] = i;
			score++;
		}
	}
	hash->score = score;
	return score;
}

static void mutate_hash(struct hash *hash, struct rng *rng, uint32_t mask)
{
	int i;

	if (hash->blameworthy_string_1 == NULL) {
		hash->blameworthy_string_1 = "dummy1";
	}
	if (hash->blameworthy_string_2 == NULL) {
		hash->blameworthy_string_2 = "dummy2";
	}

	for (i = 0 ; i < 100; i++) {
		uint32_t h1, h2;
		uint32_t p = rand_range(rng, 0, PARAM_LAST - 1);
		uint32_t b = rand_range(rng, param_odd[p], param_width[p] - 1);
		hash->params[p] ^= 1 << b;
		h1 = casefold_hash(hash, hash->blameworthy_string_1) & mask;
		h2 = casefold_hash(hash, hash->blameworthy_string_2) & mask;
		if (h1 == h2) {
			/* we have made no improvement in this case */
			hash->params[p] ^= 1 << b;
			continue;
		}
		//printf("mutated param %u after %d\n", p, i);

		hash->score = 0;
		return;
	}
	//printf("no good muations found, re-initialising\n");
	init_hash(hash, rng, mask);
}


static void replace_victim(struct hash *hashpool, struct rng *rng,
			   struct stats *stats,
			   struct hash *victim, int candidates, uint32_t mask)
{
	struct hash *hash_a;
	struct hash *hash_b;
	struct hash *hash_c;
	int i, a, b, c;
	uint64_t map;

	a = rand_range(rng, 0, POP - 1);
	b = rand_range_excl(rng, 0, POP - 1, a);
	hash_a = &hashpool[a];
	hash_b = &hashpool[b];
	if (hash_a->score < hash_b->score){
		hash_a = &hashpool[b];
		hash_b = &hashpool[a];
	}

	for (i = 0; i < candidates - 2; i++) {
		c = rand_range(rng, 0, POP - 1);
		hash_c = &hashpool[c];
		if (hash_c == hash_a || hash_c == hash_b) {
			i--;
			continue;
		}
		if (hash_c->score > hash_a->score) {
			hash_b = hash_a;
			hash_a = hash_c;
		} else if (hash_c->score > hash_b->score) {
			hash_b = hash_c;
		}
	}
	/* so we have two decent parents, hash_a and hash_b, but if they are
	   both the same we will mutate them (counter inbreeding). */
	if (memcmp(hash_a->params, hash_b->params,
		   sizeof(hash_b->params)) == 0) {
		mutate_hash(hash_a, rng, mask);
		mutate_hash(hash_b, rng, mask);
		stats->inbreds++;
	}

	map = rand64(rng);
	for (i = 0; i < PARAM_LAST; i++) {
		if (map & 1) {
			victim->params[i] = hash_a->params[i];
		} else {
			victim->params[i] = hash_b->params[i];
		}		
		map >>= 1;
	}
	mutate_hash(victim, rng, mask);	
	victim->score = 0;
}

int cmp_uint32(const void *a, const void *b)
{
	return *(uint32_t *)a - *(uint32_t *)b;
}

static void refresh_pool(struct hash *hashpool, struct rng *rng,
			 struct stats *stats, uint32_t mask,
			 uint32_t viciousness)
{
	int i;
	uint32_t doom_threshold;
	uint32_t tweak_threshold;
	uint32_t scores[POP];
	for (i = 0; i < POP; i++) {
		scores[i] = hashpool[i].score;
	}
	qsort(scores, POP, sizeof(uint32_t), cmp_uint32);
	doom_threshold = scores[POP / viciousness];
	tweak_threshold = scores[POP * (viciousness - 1) / viciousness];
	for (i = 0; i < POP; i++) {
		struct hash *hash = &hashpool[i];
		uint32_t score = hash->score;
		if (score < doom_threshold) {
			stats->victims++;
			replace_victim(hashpool, rng, stats, hash, 5, mask);
		} else if (score <= tweak_threshold) {
			stats->mutations++;
			mutate_hash(hash, rng, mask);
		}
	}
}


static void print_collisions(struct hash *hash, struct strings *s,
			     uint32_t mask)
{
	uint32_t hits[mask + 1];
	int collisions = 0;
	int i;
	memset(hits, 0, (mask + 1) * sizeof(hits[0]));
	for (i = 0; i < s->n_strings; i++) {
		uint32_t h = casefold_hash(hash, s->strings[i]) & mask;
		if (hits[h]) {
			collisions++;
			printf("collision %4x %s %s\n", h,
			       s->strings[i], s->strings[hits[h]]);
		}
		else {
			hits[h] = i;
		}
	}
	printf("%d collisions\n", collisions);
}


int main(int argc, char *argv[])
{
	uint32_t mask;
	struct rng rng;
	int i;
	int best_run = 0;
	uint64_t count = 0;
	struct timespec start, end, mid;
	struct hash hashpool[POP];
	double running_min = 0;
	double running_mean = 0;
	double running_max = 0;
	uint32_t viciousness = 3;
	uint64_t viciousness_threshold = 100000;
	struct hash best_hash;
	struct stats stats = {0};
	if (argc < 3) {
		printf("usage: %s <string list> <hash bits>\n\n",
		       argv[0]);
		printf("string list is one string per line.\n");
	}
	printf("got %s %s %s\n",
	       argv[0], argv[1], argv[2]);

	struct strings strings = load_strings(argv[1]);

	int bits = strtoul(argv[2], NULL, 10);
	mask = (1 << bits) - 1;

#if 0
	rng_init(&rng, 12345);
#else
	rng_random_init(&rng);
#endif

	for (i = 0; i < POP; i++) {
		init_hash(&hashpool[i], &rng, mask);
	}

	printf("mask %u n_strings %d\n", mask, strings.n_strings);
	clock_gettime(CLOCK_MONOTONIC, &start);
	mid = start;
	
	while (true) {
		uint32_t s;
		uint32_t sum = 0;
		uint32_t sum2 = 0;
		uint32_t mean, variance, deviation;
		uint32_t hs = 0;
		uint32_t ls = mask + 2;
		shuffle_strings(&strings, &rng);
		for (i = 0; i < POP; i++) {
			struct hash *hash = &hashpool[i];
			s = test_one_hash(hash, &strings, &rng, mask, &stats);
			sum += s;
			sum2 += s * s;
			hs = MAX(s, hs);
			ls = MIN(s, ls);
			if (s > best_run){
				printf(" score %4d:"
				       "%4d gen%6lu.%04d ",
				       s, strings.n_strings - s, count, i);
				uint32_t control = hash->params[0];
				printf("\033[01;34m%0*x ",
				       (param_width[0] + 3) / 4,
				       control);
				for (int j = 1; j < PARAM_LAST; j++) {
					uint32_t v = GET_VAL(j);
					int d = (v != 0);
					printf("\033[0%d;%dm", d, 36 + (j & 1));
					printf("%0*x ",
					       (param_width[j] + 3) / 4,
					       hash->params[j]);
				}
				printf("\033[00m\n");
				best_run = s;
				best_hash = *hash;
				if (s == strings.n_strings) {
					goto win;
				}
			}
		}
		mean = (sum + POP / 2) / POP;
		variance = (sum2 - (sum * sum) + (POP * POP / 2)) / (POP * POP);
		deviation = sqrtf(variance);
		refresh_pool(hashpool, &rng, &stats, mask, viciousness);
		count++;
		if (count % STAT_P == 0) {
			int64_t secs, nano, total;
			clock_gettime(CLOCK_MONOTONIC, &end);
			total = end.tv_sec - start.tv_sec;
			secs = end.tv_sec - mid.tv_sec;
			nano = end.tv_nsec - mid.tv_nsec;
			if (count == STAT_P) {
				running_min = ls;
				running_mean = mean;
				running_max = hs;
			}
			running_min = (running_min * 7 + ls) / 8.0;
			running_mean = (running_mean * 7 + mean) / 8.0;
			running_max = (running_max * 7 + hs) / 8.0;

			printf("\033[00;37m%4luk in %2ld:%02ld:%02ld "
			       "[+%.2fs]\033[00m "
			       "scores min %4u mean %4u max %4u dev %2u"
			       "\033[00;34m"
			       "running %4.0f %.0f %4.0f "
			       "\033[00;35m"
			       "kill %2d mut %2d inbred %2d"
			       "\033[00m\n",
			       count >> 10,
			       total / 3600, (total / 60) % 60,
			       total % 60, secs + nano * 1e-9,
			       ls, mean, hs, deviation,
			       running_min, running_mean, running_max,
			       STAT_MEAN(stats.victims),
			       STAT_MEAN(stats.mutations),
			       STAT_MEAN(stats.inbreds));
			mid = end;
			stats.victims = 0;
			stats.mutations = 0;
			stats.inbreds = 0;
		}
		if (count == viciousness_threshold) {
			viciousness++;
			viciousness_threshold *= 2;
			printf("adjusting viciousness to %u at count %lu\n",
			       viciousness, count);
		}
		
	}
  win:
	print_collisions(&best_hash, &strings, mask);
	printf("final best results\n");
	printf("run %d\n", best_run);
	printf("\n");
	return 0;
}
