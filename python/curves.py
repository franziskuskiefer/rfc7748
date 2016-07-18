#!/bin/python

p25519 = 0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffed
p448 = 0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffeffffffffffffffffffffffffffffffffffffffffffffffffffffffff

# decode little endian encoded byte array to integer
def decodeLittleEndian(b, bits = 255):
  return sum([b[i] << 8*i for i in range(int((bits+7)/8))])

# decode an incoming byte array
def decodeUCoordinate(u, bits = 255):
  u_list = [b for b in u]
  # Ignore any unused bits.
  if bits % 8:
    u_list[-1] &= (1<<(bits%8))-1
  return decodeLittleEndian(u_list, bits)

# return string
def encodeUCoordinate(u, bits = 255, p = p25519):
  u = u % p
  return ''.join([chr((u >> 8*i) & 0xff) for i in range(int((bits+7)/8))])

# return bytes
def encodeUCoordinateBytes(u, bits = 255, p = p25519):
  u = u % p
  r = []
  for i in range(int((bits+7)/8)):
    r.append((u >> 8*i) & 0xff)
  return r

# helper function to print byte arrays
def printBytes(s):
  return "".join("{:02x}".format(c) for c in s)

# decode incoming scalars (byte arrays) into an integer (25519)
def decodeScalar25519(k):
  k_list = [b for b in k]
  k_list[0] = int(k_list[0]) & 248
  k_list[31] = int(k_list[31]) & 127
  k_list[31] = int(k_list[31]) | 64
  return decodeLittleEndian(k_list, 255)

# decode incoming scalars (byte arrays) into an integer (448)
def decodeScalar448(k):
  k_list = [b for b in k]
  k_list[0] = int(k_list[0]) & 252
  k_list[55] = int(k_list[55]) | 128
  return decodeLittleEndian(k_list, 448)

# conditional swap
def cswap(swap, a, b, bits = 255):
  dummy = (2**bits-swap) & (a^b)
  a = a ^ dummy
  b = b ^ dummy
  return (a, b)

def multiply(u, k, p = p25519, a24 = 121665, bits = 255):
  x_2 = 1
  z_2 = 0

  x_3 = u
  z_3 = 1

  swap = 0
  for t in range(bits-1,-1,-1):
    k_t = (k >> t) & 1
    swap ^= k_t
    (x_2, x_3) = cswap(swap, x_2, x_3, bits)
    (z_2, z_3) = cswap(swap, z_2, z_3, bits)
    swap = k_t

    A = (x_2 + z_2) % p
    AA = pow(A, 2, p)
    B = (x_2 - z_2) % p
    BB = pow(B, 2, p)
    E = (AA - BB) % p
    x_2 = (AA * BB) % p
    z_2 = (E * (AA + (a24 * E))) %p

    C = (x_3 + z_3) % p
    D = (x_3 - z_3) % p
    CB = (C * B) % p
    DA = (D * A) % p
    x_3 = pow(DA + CB, 2, p)
    DACB2 = pow(DA - CB, 2, p)
    z_3 = (u * DACB2) % p


  (x_2, x_3) = cswap(swap, x_2, x_3, bits)
  (z_2, z_3) = cswap(swap, z_2, z_3, bits)
  r = (x_2 * pow(z_2, p - 2, p)) % p
  return r