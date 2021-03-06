#define _GNU_SOURCE 1
#include <inttypes.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

#define C_NORMAL  "\033[00m"
#define BG_NORMAL  "\043[00m"
#define C_DARK_RED  "\033[00;31m"
#define C_RED "\033[01;31m"
#define C_DARK_GREEN  "\033[00;32m"
#define C_GREEN  "\033[01;32m"
#define C_YELLOW  "\033[01;33m"
#define C_DARK_YELLOW  "\033[00;33m"
#define C_DARK_BLUE  "\033[00;34m"
#define C_BLUE  "\033[01;34m"
#define C_PURPLE  "\033[00;35m"
#define C_MAGENTA  "\033[01;35m"
#define C_DARK_CYAN  "\033[00;36m"
#define C_CYAN  "\033[01;36m"
#define C_GREY  "\033[00;37m"
#define C_WHITE  "\033[01;37m"

#define C_LT_GREEN  "\033[38;5;113m"
#define C_LILAC  "\033[38;5;141m"


#define unlikely(x) __builtin_expect(!!(x), 0)
#define likely(x) __builtin_expect(!(x), 0)

#define COLOUR(c, s) c s C_NORMAL

#ifndef MAX
#define MAX(a, b)  (((a) >= (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#endif

#define offsetof(type, member)  __builtin_offsetof (type, member)

#define ROTATE(x, k) (((x) << (k)) | ((x) >> (sizeof(x) * 8 - (k))))

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

#if 1
#include "sfc.h"
#elif 1
#include "jsf.h"
#else
#include "chacha8.h"
#endif

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

static inline struct strings load_strings(const char *filename)
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
