#include "curve25519.h"

/* helper functions */
inline uint64_t
mask(uint128_t x)
{
    return x.lo & 0x7ffffffffffff;
}

inline uint64_t
mask2(uint128_t x)
{
    return x.lo;
}

inline uint128_t
mask3(uint128_t x)
{
    uint128_t ret = { x.lo & 0x7ffffffffffff, 0 };
    return ret;
}

inline uint128_t
init128()
{
    uint128_t ret = { 0, 0 };
    return ret;
}

inline uint128_t
init128x(uint64_t x)
{
    uint128_t ret = { x, 0 };
    return ret;
}

/* arithmetic */

inline uint128_t
add128(uint128_t a, uint128_t b)
{
    uint128_t ret;
    ret.lo = a.lo + b.lo;
    ret.hi = a.hi + b.hi + (ret.lo < b.lo);
    return ret;
}

/* out = 19 * a */
inline uint128_t
mul12819(uint128_t a)
{
    uint128_t ret = lshift128(a, 4);
    ret = add128(ret, a);
    ret = add128(ret, a);
    ret = add128(ret, a);
    return ret;
}

inline uint128_t
mul6464(uint64_t a, uint64_t b)
{
    uint128_t ret;
    uint64_t t0 = ((uint64_t)(uint32_t)a) * ((uint64_t)(uint32_t)b);
    uint64_t t1 = (a >> 32) * ((uint64_t)(uint32_t)b) + (t0 >> 32);
    uint64_t t2 = (b >> 32) * ((uint64_t)(uint32_t)a) + ((uint32_t)t1);
    ret.lo = (((uint64_t)((uint32_t)t2)) << 32) + ((uint32_t)t0);
    ret.hi = (a >> 32) * (b >> 32);
    ret.hi += (t2 >> 32) + (t1 >> 32);
    return ret;
}

/* only defined for n < 64 */
inline uint128_t
rshift128(uint128_t x, uint8_t n)
{
    uint128_t ret;
    ret.lo = (x.lo >> n) + (x.hi << (64 - n));
    ret.hi = x.hi >> n;
    return ret;
}

/* only defined for n < 64 */
inline uint128_t
lshift128(uint128_t x, uint8_t n)
{
    uint128_t ret;
    ret.hi = (x.hi << n) + (x.lo >> (64 - n));
    ret.lo = x.lo << n;
    return ret;
}
