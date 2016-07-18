#!/bin/python

from curves import *
from curveTestData import *

def testDecodeScalar(s, expected, bits = 255):
  if bits == 255:
    mys = decodeScalar25519(s)
  else:
    mys = decodeScalar448(s)
  if expected == mys:
    print(" >>> decodeScalar is working")
  else:
    print(" >>> decodeScalar is NOT working")
    print("Input    s: "+printBytes(s))
    print("Decoded  s: "+str(hex(mys)))
    print("Expected s: "+str(hex(expected)))

def testDecodeUCoordinate(u, expected, bits = 255):
  myu = decodeUCoordinate(u, bits)
  if expected == myu:
    print(" >>> decodeUCoordinate is working")
  else:
    print(" >>> decodeUCoordinate is NOT working")
    print("Input    u: "+printBytes(u))
    print("Decoded  u: "+str(hex(myu)))
    print("Expected u: "+str(hex(expected)))

def decodeScalar(s, bits = 255):
  if bits == 255:
    return decodeScalar25519(s)
  else:
    return decodeScalar448(s)

def getCurveParameters(bits = 255):
  if bits == 255:
    return ("Curve25519", p25519, 121665)
  else:
    return ("Curve448", p448, 39081)

def testMultiply(u, s, expected, bits = 255, iterations = 1):
  (curve, p, a24) = getCurveParameters(bits)
  ud = u
  sd = s
  for i in range(0,iterations):
    myResult = multiply(decodeUCoordinate(ud, bits), decodeScalar(sd, bits), p, a24, bits)
    ud = sd
    sd = encodeUCoordinateBytes(myResult, bits, p)
  myResultb = encodeUCoordinate(myResult, bits, p)
  myResultbString = "".join("{:02x}".format(ord(c)) for c in myResultb)
  rString = decodeUCoordinate(expected, bits)
  if rString == myResult and myResultbString == printBytes(expected):
    print(" >>> " + curve + " is working")
  else:
    print(" >>> " + curve + " is NOT working")
    print("result  : "+myResultbString)
    print("expected: "+printBytes(expected))
    print("result  : "+str(hex(myResult)))
    print("expected: "+str(hex(rString)))

# TEST 0 - Encoding/Decoding
print("\n ---- TEST 0 - Encoding/Decoding ----\n")

print(" --- 25519 --- ")
testDecodeScalar(s1b, s1)
testDecodeScalar(s2b, s2)
testDecodeUCoordinate(u1b, u1)
testDecodeUCoordinate(u2b, u2)

# NOTE: Curve25519 is not surjective, this encoding works because the most
#       significant bit is 0, the same does not hold in general, which hence
#       does not encode to the expected byte array
myu1b = encodeUCoordinate(u1)
myu1bString = "".join("{:02x}".format(ord(c)) for c in myu1b)
if "".join("{:02x}".format(c) for c in u1b) == myu1bString:
  print(" >>> encodeUCoordinate is working")
else:
  print(" >>> encodeUCoordinate is NOT working")
  print("Input  u1: "+str(hex(u1)))
  print("Encoded u1b: "+myu1bString)
  print("Expected   : "+"".join("{:02x}".format(c) for c in u1b))

print("\n --- 448 --- ")
testDecodeScalar(s1448b, s1448, 448)
testDecodeScalar(s2448b, s2448, 448)
testDecodeUCoordinate(u1448b, u1448, 448)
testDecodeUCoordinate(u2448b, u2448, 448)

# TEST 1 - Values
print("\n ---- TEST 1 - Values ----\n")

testMultiply(u1b, s1b, r1b)
testMultiply(u2b, s2b, r2b)

testMultiply(u1448b, s1448b, r1448b, 448)
testMultiply(u2448b, s2448b, r2448b, 448)

# TEST 2 - Iterations
print("\n ---- TEST 2 - Iterations ----\n")

testMultiply(uIb, sIb, rI1b)
testMultiply(uIb, sIb, rI1000b, 255, 1000)
# 1,000,000 Iterations (you probably don't want to run this, it takes a while)
# testMultiply(uIb, sIb, rI1000000b, 255, 1000000)

testMultiply(uI448b, sI448b, rI1448b, 448)
testMultiply(uI448b, sI448b, rI1000448b, 448, 1000)
# 1,000,000 Iterations (you probably don't want to run this, it takes a while)
# testMultiply(uI448b, sI448b, rI1000000448b, 448, 1000000)