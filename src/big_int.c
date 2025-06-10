#include "big_int.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *alloc_char_arr(size_t size) {

  char *arr_c = malloc(size + 1);

  if (arr_c == NULL) {
    return NULL;
  }

  arr_c[size] = '\0';

  return arr_c;
}

static bool is_valid_string(const char *num) {

  size_t i = 0;
  while (num[i] != '\0') {
    if (num[i] < '0' || num[i] > '9') {
      return false;
    }
    i++;
  }

  return true;
}

static long long abs_long_long(long long num) { return (num < 0) ? -num : num; }

static size_t num_digit(long long num) {

  num = (num < 0) ? -num : num;

  if (num < 10)
    return 1;

  long long result = 1;

  while (num /= 10) {
    result++;
  }

  return result;
}

BigInt *bigint_create() {

  BigInt *num = malloc(sizeof(BigInt));

  if (num == NULL) {
    return NULL;
  }

  num->size = 0;
  num->sign = '+';
  num->ptr = NULL;

  return num;
}

BigInt *bigint_create_copy(BigInt *num) {

  if (num == NULL) {
    return NULL;
  }

  BigInt *c_num = bigint_create();

  if (c_num == NULL) {
    return NULL;
  }

  c_num->size = num->size;
  c_num->sign = num->sign;

  if (num->ptr == NULL) {
    c_num->ptr = NULL;
    return c_num;
  }

  char *arr = alloc_char_arr(num->size);
  memcpy(arr, num->ptr, num->size);
  c_num->ptr = arr;

  return c_num;
}

BigInt *bigint_create_from_num(long long num) {

  BigInt *l_num = bigint_create();

  if (l_num == NULL) {
    return NULL;
  }

  l_num->sign = (num < 0) ? '-' : '+';
  l_num->size = num_digit(num);

  char *arr = alloc_char_arr(l_num->size);
  snprintf(arr, l_num->size + 1, "%lld", abs_long_long(num));

  l_num->ptr = arr;

  return l_num;
}

BigInt *bigint_create_from_string(const char *str) {

  if (str == NULL) {
    return NULL;
  }

  BigInt *s_num = bigint_create();

  if (s_num == NULL) {
    return NULL;
  }

  size_t s_len = strlen(str);
  char *arr = alloc_char_arr(s_len);

  if (str[0] == '-' || str[0] == '+') {
    if (is_valid_string(str + 1)) {
      memcpy(arr, str + 1, s_len);

      s_num->size = s_len;
      s_num->sign = str[0];
      s_num->ptr = arr;
    } else {
      free(arr);
      return NULL;
    }
  } else {
    if (is_valid_string(str)) {
      memcpy(arr, str, s_len);

      s_num->size = s_len;
      s_num->sign = '+';
      s_num->ptr = arr;
    } else {
      free(arr);
      return NULL;
    }
  }

  return s_num;
}

void bigint_free(BigInt *big_int) {

  free(big_int->ptr);
  free(big_int);
}
