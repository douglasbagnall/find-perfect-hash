#CC = gcc -fprefetch-loop-arrays
CC = gcc
#CC = clang-3.9

ALL_CFLAGS = -Wall -O3 -ggdb -ffast-math -lm -march=native -pg $(CFLAGS)

all: find-perfect-hash

rng-loop-finder: %: %.c find-perfect-hash-helpers.h Makefile
	$(CC) $(ALL_CFLAGS)  $< -o $@

TAGS: find-perfect-hash.c
	rm -f $@ && (find  -name "*.[ch]" | xargs etags -a)

.c.o:
	$(CC) -c -MMD -o $@ $<  $(ALL_CFLAGS)

find-perfect-hash.c:  find-perfect-hash.h

find-perfect-hash: find-perfect-hash.o argparse/argparse.o Makefile
	$(CC) -Iargparse -Wl,-O1 $(filter %.o,$^) -lm  -I. -o $@
