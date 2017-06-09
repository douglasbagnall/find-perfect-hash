/*
Originally based on chacha-regs.c version 20080118
from the estream project.
D. J. Bernstein
Public domain.
*/

struct rng
{
	uint32_t input[16];
	uint64_t output[8];
	uint offset;
};

#define QUARTERROUND(a,b,c,d)                   \
	a += b; d = ROTATE(d ^ a, 16);		\
	c += d; b = ROTATE(b ^ c, 12);		\
	a += b; d = ROTATE(d ^ a, 8);		\
	c += d; b = ROTATE(b ^ c, 7)


static inline void salsa20_wordtobyte(struct rng *rng)
{
	uint32_t x0, x1, x2, x3, x4, x5, x6, x7, x8, x9;
	uint32_t x10, x11, x12, x13, x14, x15;
	int i;
	uint32_t *output = (uint32_t *)rng->output;
	uint32_t *input = rng->input;
	uint64_t *input64 = (uint64_t*)input;
	x0 = input[0];
	x1 = input[1];
	x2 = input[2];
	x3 = input[3];
	x4 = input[4];
	x5 = input[5];
	x6 = input[6];
	x7 = input[7];
	x8 = input[8];
	x9 = input[9];
	x10 = input[10];
	x11 = input[11];
	x12 = input[12];
	x13 = input[13];
	x14 = input[14];
	x15 = input[15];
	for (i = 0; i < 4; i++) {
		QUARTERROUND( x0, x4,  x8, x12);
                QUARTERROUND( x1, x5,  x9, x13);
                QUARTERROUND( x2, x6, x10, x14);
                QUARTERROUND( x3, x7, x11, x15);
                QUARTERROUND( x0, x5, x10, x15);
                QUARTERROUND( x1, x6, x11, x12);
                QUARTERROUND( x2, x7,  x8, x13);
                QUARTERROUND( x3, x4,  x9, x14);
        }
	output[0] = x0 + input[0];
	output[1] = x1 + input[1];
	output[2] = x2 + input[2];
	output[3] = x3 + input[3];
	output[4] = x4 + input[4];
	output[5] = x5 + input[5];
	output[6] = x6 + input[6];
	output[7] = x7 + input[7];
	output[8] = x8 + input[8];
	output[9] = x9 + input[9];
	output[10] = x10 + input[10];
	output[11] = x11 + input[11];
	output[12] = x12 + input[12];
	output[13] = x13 + input[13];
	output[14] = x14 + input[14];
	output[15] = x15 + input[15];

	input64[4]++;
	if (unlikely(input64[4] == 0)) {
		input64[5]++;
	}
	rng->offset = 0;
}

static void rng_init(struct rng *rng, uint64_t seed)
{
	uint i;
	uint32_t a = seed;
	uint32_t b = seed >> 32;
        /* The original cipher uses proper keys and IVs. We just want a simple
         * 64 bit seed, so we do something ad-hoc that tries to avoid too many
         * zeros, just in case. */
        const uint32_t *base = (uint32_t *) "expand 16-byte k";
        rng->input[0] = base[0];
        rng->input[1] = base[1];
        rng->input[2] = base[2];
        rng->input[3] = base[3];

	for (i = 4; i < 16; i += 2) {
		a *= 13;
		b *= 2017;
		a ^= b;
                b += 0xfeed;
		rng->input[i] = a;
		rng->input[i + 1] = b;
	}
	salsa20_wordtobyte(rng);
}

static uint64_t rand64(struct rng *rng)
{
	uint64_t r;
	if (rng->offset == 4) {
		salsa20_wordtobyte(rng);
	}
	r = rng->output[rng->offset];
	rng->offset++;
	return r;
}
