#include "big_int.h"
#include <stdbool.h>
#include <stddef.h>
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
static int abs_int(int num) { return (num < 0) ? -num : num; }

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

static BigInt *add_leading_zeroes(BigInt *n, int num_of_zeroes) {

  if (num_of_zeroes == 0) {
    return n;
  }

  char *new_str = alloc_char_arr(n->size + num_of_zeroes);

  if (new_str == NULL) {
    return NULL;
  }

  memset(new_str, 48, num_of_zeroes);
  memcpy(new_str + num_of_zeroes, n->ptr, n->size);

  free(n->ptr);
  n->ptr = new_str;

  return n;
}

static BigInt *remove_leading_zeroes(BigInt *n) {

  if (n->size == 0) {
    return n;
  }

  size_t num_zeroes = 0;

  for (size_t i = 0; i < n->size && n->ptr[i] == '0'; ++i) {
    num_zeroes++;
  }

  if (num_zeroes == 0) {
    return n;
  }

  size_t cleaned_size = n->size - num_zeroes;
  char *clean_str = alloc_char_arr(cleaned_size);

  if (clean_str == NULL) {
    return NULL;
  }

  memcpy(clean_str, n->ptr + num_zeroes, cleaned_size);

  free(n->ptr);
  n->ptr = clean_str;
  n->size -= num_zeroes;

  return n;
}

static BigInt *sum_with_equal_sign(BigInt *n1, BigInt *n2, BigInt *res) {

  int diff = n1->size - n2->size;

  int abs_diff = abs_int(diff);

  (diff >= 0) ? add_leading_zeroes(n2, abs_diff)
              : add_leading_zeroes(n1, abs_diff);

  char *sum_str = alloc_char_arr(n1->size + 1);

  if (sum_str == NULL) {
    return NULL;
  }

  int add_part = 0;

  int k = n1->size;

  for (int i = n1->size - 1; i >= 0; i--) {

    int d1 = n1->ptr[i] - '0';
    int d2 = n2->ptr[i] - '0';

    int s = (d1 + d2 + add_part);

    if (s > 9) {
      s -= 10;
      add_part = 1;
    } else {
      add_part = 0;
    }

    sum_str[k--] = s + '0';
  }

  sum_str[k] = (add_part == 0) ? '0' : '1';

  res->ptr = sum_str;
  res->size = n1->size + 1; // NOTE: size with leading zero
  res->sign = (n1->sign == '-') ? '-' : '+';

  return res;
}

// TODO: calculate correct sign
static BigInt *substr_with_diff_signs(BigInt *n1, BigInt *n2, BigInt *res) {

  BigInt *greater_abs = (n1->size >= n2->size) ? n1 : n2;
  BigInt *lower_abs = (n1->size < n2->size) ? n1 : n2;

  int diff = n1->size - n2->size;

  int abs_diff = abs_int(diff);

  (diff >= 0) ? add_leading_zeroes(n2, abs_diff)
              : add_leading_zeroes(n1, abs_diff);

  char *sum_str = alloc_char_arr(n1->size);

  if (sum_str == NULL) {
    return NULL;
  }

  int substr_part = 0;

  for (int i = n1->size - 1; i >= 0; i--) {

    int d_greater = greater_abs->ptr[i] - '0';
    int d_lower = lower_abs->ptr[i] - '0';

    int s = (d_greater - d_lower) - substr_part;

    if (s < 0) {
      s += 10;
      substr_part = 1;
    } else {
      substr_part = 0;
    }

    sum_str[i] = s + '0';
  }

  res->ptr = sum_str;
  res->size = n1->size;
  res->sign = (n1->sign == '-') ? '-' : '+';

  return res;
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
      memcpy(arr, str + 1, s_len - 1);

      s_num->size = s_len - 1;
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

bool bigint_less(BigInt *n1, BigInt *n2) {

  if (n1->sign == '+' && n2->sign == '-') {
    return false;
  } else if (n1->sign == '-' && n2->sign == '+') {
    return true;
  }

  int cmp = strcmp(n1->ptr, n2->ptr);

  if (n1->sign == '+' && n2->sign == '+') {
    return (cmp >= 0) ? false : true;
  }

  return (cmp <= 0) ? false : true;
}

BigInt *bigint_sum(BigInt *n1, BigInt *n2) {

  if (n1 == NULL || n2 == NULL) {
    return NULL;
  }

  BigInt *res = bigint_create();

  if (res == NULL) {
    return NULL;
  }

  if (n1->sign == n2->sign) {
    // TODO: down func can return NULL
    // add check
    sum_with_equal_sign(n1, n2, res);
    remove_leading_zeroes(res);
  } else {
    substr_with_diff_signs(n1, n2, res);
  }

  return res;
}

BigInt *bigint_difference(BigInt *n1, BigInt *n2) {

  if (n1 == NULL || n2 == NULL) {
    return NULL;
  }

  BigInt *res = bigint_create();

  if (res == NULL) {
    return NULL;
  }

  if (n1->sign != n2->sign) {
    substr_with_diff_signs(n1, n2, res);
  } else {
    sum_with_equal_sign(n1, n2, res);
    remove_leading_zeroes(res);
  }

  return res;
}

void bigint_free(BigInt *big_int) {

  free(big_int->ptr);
  free(big_int);
}
