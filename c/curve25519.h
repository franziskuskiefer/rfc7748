#include <stdint.h>
#include <stdio.h>

/* private */

#ifndef HAVE_INT128_SUPPORT
	typedef unsigned __int128 uint128_t;
	uint128_t add128(uint128_t a, uint128_t b) {
		return a + b;
	}
	uint128_t mul6464(uint64_t a, uint64_t b) {
		return (uint128_t)a * (uint128_t)b;
	}
	uint128_t mul12819(uint128_t a) {
		return (uint128_t)a * 19;
	}
	uint128_t rshift128(uint128_t x, uint8_t n) {
		return x >> n;
	}
	uint128_t lshift128(uint128_t x, uint8_t n) {
		return x << n;
	}
	uint64_t mask(uint128_t x) {
		return x & 0x7ffffffffffff;
	}
	uint64_t mask2(uint128_t x) {
		return (uint64_t)x;
	}
	uint128_t mask3(uint128_t x) {
		return x & 0x7ffffffffffff;
	}
	uint128_t init128() {
		uint128_t x; return x;
	}
	uint128_t init128x(uint64_t x) {
		return x;
	}
#else/* uint128_t for Windows */
	struct uint128_t_str
	{
	  uint64_t lo;
	  uint64_t hi;
	};
	typedef struct uint128_t_str uint128_t;
	uint128_t add128(uint128_t a, uint128_t b);
	uint128_t mul6464(uint64_t a, uint64_t b);
	uint128_t mul12819(uint128_t a);
	uint128_t rshift128(uint128_t x, uint8_t n);
	uint128_t lshift128(uint128_t x, uint8_t n);
	uint64_t mask(uint128_t x);
	uint64_t mask2(uint128_t x);
	uint128_t mask3(uint128_t x);
	uint128_t init128();
	uint128_t init128x(uint64_t x);
#endif

/* public */
int curve25519_scalarmult(uint8_t *mypublic, const uint8_t *secret, const uint8_t *basepoint);