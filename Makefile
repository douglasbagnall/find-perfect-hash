CC = gcc
#CC = clang-3.9

find-perfect-hash: find-perfect-hash.c find-perfect-hash-helpers.h Makefile
	$(CC) -fprefetch-loop-arrays -Wall -O3 $< -o $@ -ggdb -ffast-math -lm -march=native

TAGS: find-perfect-hash.c
	rm -f $@ && (find  -name "*.[ch]" | xargs etags -a)
