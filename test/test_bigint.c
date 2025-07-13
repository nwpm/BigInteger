#include "../src/big_int.h"
#include <assert.h>
#include <stdio.h>

int main() {

  BigInt *n = bigint_create_from_num(1234567);

  printf("%s\n", n->cstr);

  bigint_free(n);

  return 0;
}
