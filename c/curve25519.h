#include <stdint.h>
#include <stdio.h>

/* private */

/* public */
int curve25519_32_scalarmult(unsigned char* q, const unsigned char* n, const unsigned char* p);
int curve25519_64_scalarmult(uint8_t *mypublic, const uint8_t *secret, const uint8_t *basepoint);