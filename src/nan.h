#ifndef NAN_H_
#define NAN_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>

#define EXP_MASK (((1LL << 11LL) - 1LL) << 52LL)
// * Mantissa [0 - 51 bits] => 52 bits
#define FRACTION_MASK ((1LL << 52LL) - 1LL)
// * 63rd bit
#define SIGN_MASK 1LL << 63LL
// * First four bits of Mantissa
#define TYPE_MASK (((1LL << 4LL) - 1LL) << 48LL)
// * 1st 48 bits of mantissa where value is stored
#define VALUE_MASK ((1LL << 48LL) - 1LL)

#define TYPE(n) ((1LL << 3LL) + n)
#define DOUBLE_TYPE 0
#define INTEGER_TYPE 1
#define POINTER_TYPE 2


#define INSPECT_VALUE(type, value, label)                        \
{                                                                \
       type name = (value);                                      \
       printf("%s = \n    ", label);                             \
       print_bits((uint8_t*) &name, sizeof(name));               \
       printf("    is_nan = %d\n", is_nan(name));                \
       printf("    isnan = %d\n", isnan(name));                  \
       printf("    is_inf = %d\n", is_inf(name));                \
       printf("    isinf = %d\n", isinf(name));                  \
}                                                                \

static void print_bits_simple(int n);
static void print_bytes(uint8_t *bytes, size_t byte_size);
void print_bits(uint8_t *bytes, size_t byte_size);

int is_nan(double x);
int is_inf(double x);
uint64_t get_type(double x);
uint64_t get_value(double x);

double set_type(double x, uint64_t type);
double set_value(double x, uint64_t value);

double mk_inf(void);

int is_double(double x);
int is_integer(double x);
int is_pointer(double x);

double as_double(double x);
uint64_t as_integer(double x);
void *as_pointer(double x);

double box_double(double x);
double box_integer(uint64_t x);
double box_pointer(void* x);

#endif // NAN_H_

#ifdef NAN_IMPLEMENTATION

static void print_bits_simple(int n) {
    printf("%d = ", n);
    for (int i = 7; i >= 0; --i) {
      printf("%d", !!(n & (1 << i)));
    }
    printf("\n");
}

static void print_bytes(uint8_t *bytes, size_t byte_size) {
    printf("byte_size: %zu\n", byte_size);
    for(size_t i = 0; i < byte_size; ++i) {
      printf("0x%X ", bytes[i]);
    }    
}

void print_bits(uint8_t *bytes, size_t byte_size) {
  for (int i = (int)byte_size - 1; i >= 0; --i) {
    uint8_t byte = bytes[i];
    // printf("0x%X ", byte);
    for (int j = 7; j >= 0; --j) {
	    printf("%d", !!(byte & (1 << j)));
    }
    printf(" ");
  }
  printf("\n");
}  

int is_nan(double x) {
    uint64_t y = (*(uint64_t*) &x);
    // * since for NaN FRACTION_MASK is always non-zero because 1st bit of mantissa is set
    // * is always set to 1 => 1000 00000000 00000000 00000000 00000000 00000000 00000000
    return ((y & EXP_MASK) == EXP_MASK) && ((y & FRACTION_MASK) != 0);
}

int is_inf(double x) {
    uint64_t y = (*(uint64_t*)&x);
    // * Here the FRACTION_MASK is always zero
    // * 01111111 11110000 00000000 00000000 00000000 00000000 00000000 00000000 
    return ((y & EXP_MASK) == EXP_MASK) && ((y & FRACTION_MASK) == 0);
}

uint64_t get_type(double x) {
    uint64_t y = (*(uint64_t*) &x);
    return (y & TYPE_MASK) >> 48LL;
}

uint64_t get_value(double x) {
    uint64_t y = *(uint64_t*) &x;
    return (y & VALUE_MASK);    
}

double set_type(double x, uint64_t type) {
    uint64_t y = *(uint64_t *)&x;
    y = (y & (~TYPE_MASK)) | (((TYPE_MASK >> 48LL) & type) << 48LL);
    return *(double *)&y;
}

double set_value(double x, uint64_t value) {
    uint64_t y = *(uint64_t*) &x;
    y = (y & (~VALUE_MASK)) | (value & VALUE_MASK);
    return *(double*)&y; 
}

double mk_inf(void) {
    uint64_t y = EXP_MASK;
    return *(double *) &y;
}

int is_double(double x) {
    return !isnan(x);
}

int is_integer(double x) {
    return isnan(x) && get_type(x) == TYPE(INTEGER_TYPE);
}

int is_pointer(double x) {
  return isnan(x) && get_type(x) == TYPE(POINTER_TYPE);
}

double as_double(double x) {
  //    return *(double*) get_value(x);
  return x;
}

uint64_t as_integer(double x) {
  return  get_value(x);
}

void *as_pointer(double x) {
  return (void*) get_value(x);
}

double box_double(double x) {
  return x;
  //  return set_value(set_type(mk_inf(), TYPE(DOUBLE_TYPE)), x);
}

double box_integer(uint64_t x) {
  return set_value(set_type(mk_inf(), TYPE(INTEGER_TYPE)), x);
}

double box_pointer(void* x) {
  return set_value(set_type(mk_inf(), TYPE(POINTER_TYPE)), (uint64_t)  x);
}

#endif // NAN_IMPLEMENTATION