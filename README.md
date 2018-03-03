Searching for perfect hashes
----------------------------

A method for converting a simple standard hash (e.g. FNV or DJB) into
a perfect hash for a particular set of strings.

For example, running

```./find-perfect-hash ldap-100.txt -b 7 -e 100000000 --processes=2 -C```

might find this function:

```
uint hash(const char *string)
{
	const char *s;
	uint32_t h = 2166136261;
	uint r = 0;
	for (s = string; s != '\0'; s++) {
		uint8_t c = (uint8_t)*s;
		h ^= c;
		h *= 16777619;
	}
	r |= ((h *      0xac9617e6d3ULL) >> 39) &    0x7;
	r |= ((h *  0x1c5bd8978f8a05ULL) >> 49) &    0x8;
	r |= ((h *           0x676b1ULL) >> 14) &   0x10;
	r |= ((h *    0xca8366153fb1ULL) >> 43) &   0x20;
	r |= ((h *  0x247e5bb9c2c8d3ULL) >> 48) &   0x40;
	return r;
}
```

which is a 7-bit perfect hash for the 100 words in `ldap-100.txt`.
Other strings will be hashed to random buckets.

In the above function, the lines not starting `r |=` are the standard
FNV hash; what `find-perfect-hash` does is find the parameters in
those lines.

License
-------

GPLv3 applies to this repository, but *not* to the code you generate
using it. That is yours.
