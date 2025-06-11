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
BigInt *bigint_create_copy(BigInt *num);

bool bigint_less(BigInt *n1, BigInt *n2);

BigInt *bigint_sum(BigInt *n1, BigInt *n2);
BigInt *bigint_difference(BigInt *n1, BigInt *n2);

void bigint_free(BigInt *big_int);

#endif // BIG_INT_H
