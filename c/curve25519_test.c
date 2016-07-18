
#include "curve25519.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

const char *sk1 = "a546e36bf0527c9d3b16154b82465edd62144c0ac1fc5a18506a2244ba449ac4";
const char *pk1 = "e6db6867583030db3594c1a424b15f7c726624ec26b3353b10a903a6d0ab1c4c";
const char *expectedResult1 = "c3da55379de9c6908e94ea4df28d084f32eccf03491c71f754b4075577a28552";

const char *sk2 = "4b66e9d4d1b4673c5ad22691957d6af5c11b6421e0ea01d42ca4169e7918ba0d";
const char *pk2 = "e5210f12786811d3f4b7959d0538ae2c31dbe7106fc03c3efc4cd549c715a493";
const char *expectedResult2 = "95cbde9476e8907d7aade45cb4b873f88b595a68799fa152e6f8f7647aac7957";

const char *skA = "77076d0a7318a57d3c16c17251b26645df4c2f87ebc0992ab177fba51db92c2a";
const char *pkA = "8520f0098930a754748b7ddcb43ef75a0dbf3a0d26381af4eba4a98eaa9b4e6a";
const char *skB = "5dab087e624a8a4b79e17f8b83800ee66f3bb1292618b6fd1c2f8b27ff88e0eb";
const char *pkB = "de9edb7d7b7dc1b4d35b61c2ece435373f8343c85b78674dadfc7e146f882b4f";
const char *sharedAB = "4a5d9d5ba4ce2de1728e3bf480350f25e07e21c947d19e3376f09b3c1e161742";

/* skIt = pkIt is also used as base point */
const char *skIt = "0900000000000000000000000000000000000000000000000000000000000000";
const char *pkIt = "0900000000000000000000000000000000000000000000000000000000000000";
const char *expectedResultIt1 = "422c8e7a6227d7bca1350b3e2bb7279f7897b87bb6854b783c60e80311ae3079";
const char *expectedResultIt1000 = "684cf59ba83309552800ef566f2f4d3c1c3887c49360e3875f2eb94d99532c51";
const char *expectedResultIt1000000 = "7c3911e0ab2586fd864497297e575e6f3bc601c0883c30df5f4dd2d24f665424";

const char *expectedIntermediate1000 = "b54931505707f5468ca13d1aaca2fbd9932a0fd52105946712bd2f97bbd35858";
const char *expectedIntermediate2000 = "76bdbccf899c51d6552deff9dc68fb2b08c007c0dfdafd1c492af02f8e14b142";
const char *expectedIntermediate3000 = "7f5cbd62d914b80d516efb6454a2ea218112c3bed26cc7f9eaa281a7578ecf63";

static int bit32 = 1;

void
hexString2ByteArray(unsigned char *item, const char *str)
{
    int i = 0;
    int byteval = 0;
    int tmp = strlen(str);

    if ((tmp % 2) != 0) {
        return;
    }

    while (str[i]) {
        if ((str[i] >= '0') && (str[i] <= '9')) {
            tmp = str[i] - '0';
        } else if ((str[i] >= 'a') && (str[i] <= 'f')) {
            tmp = str[i] - 'a' + 10;
        } else if ((str[i] >= 'A') && (str[i] <= 'F')) {
            tmp = str[i] - 'A' + 10;
        } else {
            return;
        }

        byteval = byteval * 16 + tmp;
        if ((i % 2) != 0) {
            (item)[i / 2] = byteval;
            byteval = 0;
        }
        i++;
    }
}

void
printChars(char *name, unsigned char *X, int l)
{
    printf("%s: 0x", name);
    for (int i = 0; i < l; ++i) {
        printf("%02x", X[i]);
    }
    printf("\n");
}

void
printScalar(char *name, unsigned char *s)
{
    printChars(name, s, 32);
}

void
printEncodedPoint(char *name, unsigned char *p)
{
    printChars(name, p, 32);
}

void
printPoint(char *name, uint32_t *p)
{
    printf("%s: ", name);
    for (int i = 0; i < 8; ++i) {
        printf("0x%08x ", p[i]);
    }
    printf("\n");
}

void
testIt(const char *name, const char *scalar, const char *point, const char *expected, int iterations)
{
    unsigned char x[32];
    unsigned char p[32];
    unsigned char exp[32];
    hexString2ByteArray(&x[0], scalar);
    hexString2ByteArray(&p[0], point);
    hexString2ByteArray(&exp[0], expected);

    unsigned char result[32];
    for (int i = 0; i < iterations; ++i) {
        printf("\r %d", i);
        fflush(stdout);
        if (bit32){
            curve25519_32_scalarmult(result, x, p);
        } else {
            curve25519_64_scalarmult(result, x, p);
        }
        // if (i != 0 && i%1000 == 0) {
        //     printf("\n%d - ", i);
        //     printEncodedPoint("result", result);
        // }
        memcpy(p, x, 32);
        memcpy(x, result, 32);
    }
    if (memcmp(result, exp, 32) != 0) {
        printf(" >>> %s failed\n", name);
        printEncodedPoint("result", result);
        printEncodedPoint("expect", exp);
        exit(1);
    }
    printf(" >>> %s succeeded\n", name);
}

int
main(int argc, char *argv[])
{
    struct timeval t1, t2;
    double elapsedTime;

    if (argc > 1 && strncmp(argv[1], "64", 2) == 0) {
        bit32 = 0;
    }

    testIt("sk1", sk1, pk1, expectedResult1, 1);
    testIt("sk2", sk2, pk2, expectedResult2, 1);

    testIt("iteration 1", skIt, pkIt, expectedResultIt1, 1);

    gettimeofday(&t1, NULL);
    testIt("iteration 1000", skIt, pkIt, expectedResultIt1000, 1000);
    gettimeofday(&t2, NULL);
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
    elapsedTime /= 1000;
    printf("Speed: %f ms/op\n", elapsedTime);

    // testIt("iteration 1000000", skIt, pkIt, expectedResultIt1000000, 1000000);

    return 0;
}