#define _GNU_SOURCE 1
#include <inttypes.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

#ifndef MAX
#define MAX(a, b)  (((a) >= (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#endif

#define offsetof(type, member)  __builtin_offsetof (type, member)

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

static inline void rng_random_init(struct rng *rng)
{
	/* random enough for this */
	struct timespec t;
	uint64_t seed;
	clock_gettime(CLOCK_REALTIME, &t);
	seed = ((uint64_t)t.tv_nsec << 20) + t.tv_sec;
	seed ^= (uintptr_t)rng;
	seed += (uintptr_t)&rng_random_init;
	printf("seeding with %lu\n", seed);
	rng_init(rng, seed);
}

static inline uint32_t rand_range(struct rng *rng, uint32_t low, uint32_t high)
{
	/* inclusive of top number, just to be different */
	uint64_t r = rand64(rng);
	return low + r % (high - low + 1);
}


#define ALIGNMENT 32
static inline __attribute__((__malloc__, __assume_aligned__(ALIGNMENT),
        __returns_nonnull__)) void *
malloc_aligned(size_t size) {
	void *mem;
	posix_memalign(&mem, ALIGNMENT, size);
	return mem;
}

#ifndef __clang__
#define ASSUME_ALIGNED(x)   (x) = __builtin_assume_aligned ((x), ALIGNMENT)
#else
#define ASSUME_ALIGNED(x) /* x */
#endif

static int __attribute__((unused))
pgm_dump_double(const double *matrix, uint width, uint height,
		const char *name, double min, double max)
{
	int i;
	double high = -1e299;
	double low = 1e299;
	size_t size = width * height;
	FILE *fh = fopen(name, "w");
	if (fh == NULL){
		printf("could not open '%s' for writing", name);
		return 1;
	}
	fprintf(fh, "P5\n%u %u\n255\n", width, height);

	for (i = 0; i < width * height; i++){
		low = MIN(low, matrix[i]);
		high = MAX(high, matrix[i]);
	}
	if (low < min) {
		low = min;
	}
	if (high > max) {
		high = max;
	}

	double scale = 255.99 / (high - low);
	uint8_t *im = malloc(size);
	for (i = 0; i < width * height; i++){
		double x = (matrix[i] - low) * scale;
		if (x <= 0) {
			im[i] = 0;
		} else if (x >= 255) {
			im[i] = 255;
		} else {
			im[i] = (uint8_t)x;
		}
	}
	size_t wrote = fwrite(im, 1, size, fh);
	if (wrote != size){
		printf("wanted to write %zu bytes; fwrite said %zu\n",
		       size, wrote);
		return 1;
	}
	fflush(fh);
	fclose(fh);
	free(im);
	return 0;
}



/*pbm for bitmap */
static int __attribute__((unused))
pbm_dump(const void *void_data,
	 const int width,
	 const int height,
	 const int offset,
	 const int stride,
	 const char *name)
{
	const uint8_t *data = void_data;
	if (width & 7){
		fprintf(stderr, "sorry, pbm writing for 8-aligned widths only "
			"(not %d)\n", width);
		return 1;
	}
	FILE *fh = fopen(name, "w");
	if (fh == NULL){
		fprintf(stderr, "could not open '%s' for writing\n", name);
		return 1;
	}

	fprintf(fh, "P4\n%u %u\n", width, height);
	uint8_t byte = 0;
	const uint8_t *row = data + offset;
	for (uint y = 0; y < height; y++){
		for (uint x = 0; x < width / 8; x++){
			byte = row[x];
			/* reverse the bits, from bit twiddling hacks page */
			byte = (byte * 0x0202020202ULL & 0x010884422010ULL) % 1023;
			putc(~byte, fh);
		}
		row += stride;
	}
	fflush(fh);
	fclose(fh);
	return 0;
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
		fclose(f);
		return s;
	}
	read = fread(s.mem, 1, len, f);
	if (read != len) {
		printf("gah, C file handling, wanted %d, got %lu\n",
		       len, read);
		free(s.mem);
		fclose(f);
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
	fclose(f);
	return s;
}

#define QUOTE_(x) #x
#define QUOTE(x) QUOTE_(x)


#if 1
#define START_TIMER(name) struct timespec _time_start_ ## name; \
	clock_gettime(CLOCK_MONOTONIC, &_time_start_ ## name)

#define PRINT_TIMER(name) do {						\
	struct timespec _time_end_ ## name;				\
	clock_gettime(CLOCK_MONOTONIC, &_time_end_ ## name);		\
	time_t secs = _time_end_ ## name.tv_sec - _time_start_ ## name.tv_sec;	\
	long nano = _time_end_ ## name.tv_nsec - _time_start_ ## name.tv_nsec;	\
	double elapsed = secs + 1e-9 * nano;				\
	printf("%s took %.4fs\n", QUOTE(name), elapsed);		\
	} while (0)
#else
#define START_TIMER(name) /* name */
#define PRINT_TIMER(name) /* name */
#endif
