#ifndef BIG_INT_H
#define BIG_INT_H

#include <stddef.h>

#define BIGINT_START_CAPACITY 4

typedef struct BigInt {

  char sign;
  char *cstr;
  size_t size;
  size_t capacity;

} BigInt;

BigInt *bigint_create();
BigInt *bigint_create_from_num(long long src_num);
BigInt *bigint_create_from_cstr(const char *src_cstr);
BigInt *bigint_create_copy(const BigInt *src_num);

int bigint_less(const BigInt *lhs, const BigInt *rhs);
int bigint_greater(const BigInt *lhs, const BigInt *rhs);
int bigint_less_or_equal(const BigInt *lhs, const BigInt *rhs);
int bigint_greater_or_equal(const BigInt *lhs, const BigInt *rhs);
int bigint_is_equal(const BigInt *lhs, const BigInt *rhs);

BigInt *bigint_assign(BigInt *target, const BigInt *source);
BigInt *bigint_difference(BigInt *target, const BigInt *source);
BigInt *bigint_multiply(BigInt *target, unsigned int mult_by);

BigInt *bigint_negate(BigInt *bigint_num);
BigInt *bigint_abs(BigInt *bigint_num);

void bigint_free(BigInt *bigint_num);

#endif // BIG_INT_H
