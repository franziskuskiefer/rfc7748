
SRCS = curve25519_test.c curve25519_64.c
DEFINE =

ifndef HAVE_INT128_SUPPORT
	SRCS += uint128.c
	DEFINE += -DHAVE_INT128_SUPPORT
endif

all:
	clang -g $(DEFINE) $(SRCS) -o test -lprofiler

clean:
	rm -rf test