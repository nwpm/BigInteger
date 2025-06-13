#ifndef BIG_INT_H
#define BIG_INT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct BigInt {

  char sign;
  char *ptr;
  size_t size;

} BigInt;

BigInt *bigint_create();
BigInt *bigint_create_from_num(long long num);
BigInt *bigint_create_from_string(const char *str);
BigInt *bigint_create_copy(const BigInt *num);

bool bigint_less(const BigInt *n1, const BigInt *n2);
bool bigint_is_equal(const BigInt *n1, const BigInt *n2);

BigInt *bigint_sum(const BigInt *n1, const BigInt *n2);
BigInt *bigint_difference(const BigInt *n1, const BigInt *n2);
BigInt *bigint_multiply(const BigInt *n1, unsigned int n2);

void bigint_sum_in_param(BigInt **n1, const BigInt *n2);
void bigint_free(BigInt *big_int);

#endif // BIG_INT_H
