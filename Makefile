#CC = gcc -fprefetch-loop-arrays
CC = gcc
#CC = clang-3.9

find-perfect-hash: find-perfect-hash.c find-perfect-hash-helpers.h Makefile
	$(CC) -Wall -O3 $< -o $@ -ggdb -ffast-math -lm -march=native -pg

TAGS: find-perfect-hash.c
	rm -f $@ && (find  -name "*.[ch]" | xargs etags -a)
