
find-perfect-hash: find-perfect-hash.c
	gcc -Wall -O3 $^ -o $@ -ggdb -ffast-math -lm -march=native

TAGS: find-perfect-hash.c
	rm -f $@ && (find  -name "*.[ch]" | xargs etags -a)
