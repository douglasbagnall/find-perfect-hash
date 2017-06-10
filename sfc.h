/* Algorithm by Chris Doty-Humphrey, reimplemented by Douglas Bagnall.

Original ound at http://pracrand.sourceforge.net/ with "I, Chris
Doty-Humphrey, release all of my work on PractRand to the public domain."
*/


struct rng
{
	uint64_t a, b, c, counter;
};


static uint64_t rand64(struct rng *rng)
{
	uint64_t a = rng->a;
	uint64_t b = rng->b;
	uint64_t c = rng->c;
	uint64_t r = a + b + rng->counter;
	a = b ^ (b >> 11);
	b = c + (c << 3);
	c = ROTATE(c, 24) + r;
	rng->a = a;
	rng->b = b;
	rng->c = c;
	rng->counter++;
	return r;
}

static void rng_init(struct rng *rng, uint64_t seed)
{
	int i;
	rng->a = seed;
	rng->b = seed;
	rng->c = seed;
	rng->counter = 0;
	for (i = 0; i < 12; i++) {
		rand64(rng);
	}
}
