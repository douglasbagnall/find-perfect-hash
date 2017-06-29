#define BASE_N 3
#define DETERMINISTIC 0
#define MAX_SMALL_TUPLE 200

#define N_CLOSE_PARAMS 512
#define BEST_PARAM_CHANCE (N_CLOSE_PARAMS >> 3)
#define CLOSE_PARAM_RANGE ((1 << 15) - 1)

struct hashdata {
	const char *string;
	unsigned int stringlen;
	uint64_t raw_hash;
	uint32_t running_hash;
};

struct close_param {
	uint64_t param;
	uint score;
};

struct hashcontext {
	struct hashdata *data;
	uint n;
	uint16_t *hits;
	uint n_params;
	uint bits;
	struct rng *rng;
	char *string_mem;
	const char *db_name;
	uint64_t *good_params;
	uint n_good_params;
	bool case_insensitive;
	uint hash_id;
	struct close_param *close_params;
};

struct multi_rot {
	uint64_t *params;
	uint collisions;
};

struct hash_big_tuple {
	uint64_t array[MAX_SMALL_TUPLE];
	uint n;
};

/* tuple size is always known implicitly from without */
struct tuple_list {
	uint64_t *raw;
	uint n;
};

struct hash_tuples {
	struct tuple_list tuples[MAX_SMALL_TUPLE + 1];
	uint max_size;
};


#if 1
#define MR_ROT(x) ((x) & (uint64_t)63)
#define MR_MUL(x) (((x) >> 5) | 1)
#define MR_ROT_STEP 1
#define MR_ROT_MASK 63UL
#define MUL_ROT_TO_PARAM(m, r)((((m) & ~1ULL) << 5) | (r))
#else
#define MR_ROT(x) ((x) >> 58UL)
#define MR_MUL(x) (((x) << 1) | 1)
#define MR_ROT_STEP (1ULL << 58)
#define MR_ROT_MASK (63UL << 58)
#define MUL_ROT_TO_PARAM(m, r)(((m) >> 1) | ((r) << 58))
#endif

#define MR_MASK(i) (((i) == 0) ?					\
		    (1 << BASE_N) - 1 :					\
		    (1 << (BASE_N + (i) - 1)))

#define MR_COMPONENT(x, mul, rot, mask)(ROTATE((x * mul), rot) & mask)
