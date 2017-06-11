/* Jenkin's Small Fast RNG (64 bit version) */

struct rng {
	uint64_t a;
	uint64_t b;
	uint64_t c;
	uint64_t d;
};

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
