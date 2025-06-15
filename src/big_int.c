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

static bool is_zero_str(const char *str) {

  for (size_t i = 0; str[i] != '\0'; ++i) {
    if (str[i] != '0') {
      return false;
    }
  }

  return true;
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
  n->size = n->size + num_of_zeroes;

  return n;
}

static BigInt *remove_leading_zeroes(BigInt *n) {

  if (n->size == 0) {
    return n;
  }

  if (is_zero_str(n->ptr)) {
    free(n->ptr);
    char *zero_str = alloc_char_arr(1);
    zero_str[0] = '0';

    n->ptr = zero_str;
    n->size = 1;
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

static bool abs_less(const BigInt *n1, const BigInt *n2) {

  if (n1->size > n2->size) {
    return false;
  } else if (n2->size > n1->size) {
    return true;
  }

  int cmp = strcmp(n1->ptr, n2->ptr);
  return (cmp < 0) ? true : false;
}

static BigInt *sum(BigInt *n1, BigInt *n2, BigInt *res, char res_sign) {

  BigInt *greater_abs = NULL;
  BigInt *lower_abs = NULL;

  if (abs_less(n1, n2)) {
    greater_abs = n2;
    lower_abs = n1;
  } else {
    greater_abs = n1;
    lower_abs = n2;
  }

  int diff = greater_abs->size - lower_abs->size;

  add_leading_zeroes(lower_abs, diff);

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

  sum_str[k] = (add_part) ? '1' : '0';

  res->ptr = sum_str;
  res->size = n1->size + 1; // NOTE: actual size
  res->sign = res_sign;

  (diff > 0) ? remove_leading_zeroes(n2) : remove_leading_zeroes(n1);
  remove_leading_zeroes(res);

  return res;
}

BigInt *bigint_abs(const BigInt *n) {

  if (n == NULL) {
    return NULL;
  }

  BigInt *res = bigint_create_copy(n);

  if (res == NULL) {
    return NULL;
  }

  res->sign = '+';

  return res;
}

static BigInt *difference(BigInt *n1, BigInt *n2, BigInt *res) {

  int n1_less = abs_less(n1, n2);

  BigInt *greater_abs = n1_less ? n2 : n1;
  BigInt *lower_abs = n1_less ? n1 : n2;

  int diff = greater_abs->size - lower_abs->size;
  add_leading_zeroes(lower_abs, diff);

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
  res->size = strlen(sum_str);
  res->sign = n1_less ? (n1->sign == '+' ? '-' : '+') : n1->sign;

  remove_leading_zeroes(lower_abs);
  remove_leading_zeroes(res);

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

BigInt *bigint_create_copy(const BigInt *num) {

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

static void create_from_string(BigInt **n, const char *str, char sign) {

  if (!is_valid_string(str)) {
    free(*n);
    *n = NULL;
    return;
  }

  size_t s_len = strlen(str);
  char *arr = alloc_char_arr(s_len);

  memcpy(arr, str, s_len);

  (*n)->size = s_len;
  (*n)->sign = sign;
  (*n)->ptr = arr;
}

BigInt *bigint_create_from_string(const char *str) {

  if (str == NULL) {
    return NULL;
  }

  BigInt *s_num = bigint_create();

  if (s_num == NULL) {
    return NULL;
  }

  if (str[0] == '-' || str[0] == '+') {
    const char *magnitude = str + 1;
    create_from_string(&s_num, magnitude, str[0]);
  } else {
    create_from_string(&s_num, str, '+');
  }

  return s_num;
}

bool bigint_less(const BigInt *n1, const BigInt *n2) {

  if (n1 == NULL || n2 == NULL) {
    return NULL;
  }

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

bool bigint_less_or_equal(const BigInt *n1, const BigInt *n2) {

  if (!bigint_is_equal(n1, n2)) {
    if (!bigint_less(n1, n2)) {
      return false;
    }
  }

  return true;
}

bool bigint_greater(const BigInt *n1, const BigInt *n2) {
  return !bigint_less(n1, n2);
}

bool bigint_greater_or_equal(const BigInt *n1, const BigInt *n2) {

  if (!bigint_is_equal(n1, n2)) {
    if (bigint_less(n1, n2)) {
      return false;
    }
  }

  return true;
}

bool bigint_is_equal(const BigInt *n1, const BigInt *n2) {

  if (n1 == NULL || n2 == NULL) {
    return true;
  }

  bool is_equal_size = n1->size == n2->size;
  bool is_equal_str = !strcmp(n1->ptr, n2->ptr);
  bool is_equal_sign = n1->sign == n2->sign;

  if (is_equal_sign && is_equal_size && is_equal_str) {
    return true;
  }

  return false;
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
    sum(n1, n2, res, n1->sign);
  } else {
    difference(n1, n2, res);
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

  if (n1->sign == n2->sign) {
    difference(n1, n2, res);
  } else if (n1->sign == '+' && n2->sign == '-') {
    sum(n1, n2, res, '+');
  } else if (n1->sign == '-' && n2->sign == '+') {
    sum(n1, n2, res, '-');
  }

  return res;
}

void bigint_negate(BigInt *n1) { n1->sign = (n1->sign == '+') ? '-' : '+'; }

void bigint_free(BigInt *big_int) {

  free(big_int->ptr);
  free(big_int);
}
