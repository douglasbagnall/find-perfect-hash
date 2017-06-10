#include "find-perfect-hash-helpers.h"


int main(int argc, char *argv[])
{
	uint64_t seed = 12345;
	uint64_t i, j;
	bool auto_seed = false;
	uint64_t start[8];
	uint64_t max = UINT64_MAX;
	if (argc > 3) {
		printf("usage: %s [seed [bits]]\n", argv[0]);
		exit(1);
	} else if (argc == 1) {
		auto_seed = true;
	} else {
		char *end;
		seed = strtoull(argv[1], &end, 10);
		if (end == argv[1]) {
			auto_seed = true;
		}
		if (argc == 3) {
			uint64_t b = strtoull(argv[2], &end, 10);
			if (end == argv[2] || b > 64) {
				printf("invalid number of bits: %s\n", argv[2]);
			} else if (b != 64) {
				max = (1ULL << b) - 1;
			}
		}
	}

	struct rng rng;
	if (auto_seed) {
		printf("using automatic seed\n");
		rng_random_init(&rng);
	} else {	
		printf("using seed %lu\n", seed);
		rng_init(&rng, seed);
	}

	if (sizeof(struct rng) == 32) {
		printf("we seem to be using JSF generator\n");
	} else if (sizeof(struct rng) > 128) {
		printf("probably the chacha8 generator (size %zu)\n",
		       sizeof(struct rng));
	} else {
		printf("unknown generator of size %zu\n", sizeof(struct rng));
	}
	
	for (i = 0; i < 8; i++) {
		start[i] = rand64(&rng);
		printf("start %lu: %lx\n", i, start[i]);
	}

	j = 0;
	for (i = 0; i < max; i++) {
		uint64_t c = rand64(&rng);
		if (unlikely(c == start[j])) {
			printf("partial collision %lu: %lx @ %lu\n", j, c, i);
			j++;
			if (j == 8) {
				printf("LOOP found at %lu\n", i - 8);
				exit(0);
			}
		} else {
			j = 0;
		}
	}
	printf("NO LOOP found in first %lu iterations\n", max);
}
	
