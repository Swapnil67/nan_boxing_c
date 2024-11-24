#define NAN_IMPLEMENTATION
#include "./nan.h"

int main(void) {
  double pi = 3.14159265359;
  INSPECT_VALUE(double, 1.0 / 0.0, "inf");
  INSPECT_VALUE(double, -1.0 / 0.0, "-inf");
  INSPECT_VALUE(double, mk_inf(), "inf");
  INSPECT_VALUE(double, box_double(pi), "box_double(pi)");
  INSPECT_VALUE(double, box_integer(10), "box_integer(10)");
  INSPECT_VALUE(double, box_pointer(&pi), "box_pointer(&pi)");
  
  // * testcases
  assert(pi	== as_double(box_double(pi)));
  assert(10	== as_integer(box_integer(10)));
  assert(&pi	== as_pointer(box_pointer(&pi)));
  
  printf("OK\n");
  
  return 0;
}

// * Any double whose exponent bits are all set is a NaN, regardless of the mantissa bits.
// * Signalling NaN
// * They are intended to be the result of erroneous computations, like division by zero
// * Quiet NaN
// * Every double with all of its exponent bits set and its highest mantissa bit set is a quiet NaN.


// * 32 bit SP = s 1111111 1000 0000 0000 0000 0000 0000
// * 64 bit DP = s 11111111111 1000 00000000 00000000 00000000 00000000 00000000 00000000
