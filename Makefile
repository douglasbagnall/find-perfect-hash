
find-perfect-hash: find-perfect-hash.c
	gcc -Wall -O3 $^ -o $@ -ggdb -ffast-math -lm -march=native
