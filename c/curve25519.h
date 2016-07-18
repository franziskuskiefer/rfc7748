#include <stdint.h>
#include <stdio.h>

/*
 * TYPEDEFS
 * A felem (field element) is an array of 8 32 bit numbers
 * felem x := x[0]*2^(32*7) + ... + x[7]*2^(32*0)
 */
// typedef uint32_t felem[8];
// typedef uint32_t felemA[9];
// typedef uint32_t felemM[17];
// typedef unsigned char scalar[32];
// typedef unsigned char point[32];

/* private */

/* public */
int curve25519_32_scalarmult(unsigned char* q, const unsigned char* n, const unsigned char* p);
int curve25519_64_scalarmult(uint8_t *mypublic, const uint8_t *secret, const uint8_t *basepoint);