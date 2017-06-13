/* Chris Doty-Humphrey's adaptation of Bob Jenkin's Small Fast RNG, with a
 counter to avoid (rare but theoretically possible) short cycles. */

struct rng {
	uint64_t a;
	uint64_t b;
	uint64_t c;
	uint64_t d;
	uint64_t i;
	
};

static uint64_t rand64(struct rng *x)
{
	uint64_t e = x->a - ROTATE(x->b, 45);
	x->a = x->b ^ ROTATE(x->c, 13);
	x->b = x->c + ROTATE(x->d, 37);
	x->c = x->d + e + x->i;
	x->d = e + x->a;
	x->i++;
	return x->d;
}

static void rng_init(struct rng *x, uint64_t seed)
{
	int i;
	x->a = seed;
	x->b = 1;
	x->c = 2;
	x->d = 3;
	x->i = 13;
	for (i = 0; i < 12; ++i) {
		(void)rand64(x);
	}
}
