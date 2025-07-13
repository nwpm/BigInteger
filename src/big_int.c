#include "big_int.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *_alloc_cstr(size_t size) {

  char *arr_c = malloc(size + 1);

  if (arr_c == NULL) {
    return NULL;
  }

  arr_c[size] = '\0';

  return arr_c;
}

static int _is_valid_cstr(const char *cstr) {

  size_t i = 0;

  while (cstr[i] != '\0') {
    if (cstr[i] < '0' || cstr[i] > '9') {
      return 0;
    }
    i++;
  }

  return 1;
}

static inline long long _abs(long long num) { return (num < 0) ? -num : num; }

static size_t _get_number_of_digits(long long num) {

  // num = (num < 0) ? -num : num;

  if (num >= 0 && num < 10)
    return 1;

  long long result = 1;

  while (num /= 10)
    result++;

  return result;
}

/*static bool _is_zero_str(const char *str) {

  for (size_t i = 0; str[i] != '\0'; ++i) {
    if (str[i] != '0') {
      return false;
    }
  }

  return true;
}*/

static int _create_from_cstr(BigInt *n, const char *cstr, char sign) {

  if (!_is_valid_cstr(cstr)) {
    return 0;
  }

  size_t cstr_len = strlen(cstr);

  char *alloc_cstr = strdup(cstr);

  if (!alloc_cstr) {
    return 0;
  }

  n->cstr = alloc_cstr;
  n->size = cstr_len;
  n->sign = sign;

  return 1;
}

static BigInt *_add_leading_zeroes(BigInt *num, size_t num_of_zeroes) {

  if (num_of_zeroes == 0)
    return num;

  size_t new_size = num->size + num_of_zeroes;
  char *new_str = _alloc_cstr(new_size);

  if (new_str == NULL)
    return NULL;

  memset(new_str, '0', num_of_zeroes);
  memcpy(new_str + num_of_zeroes, num->cstr, num->size);

  free(num->cstr);
  num->cstr = new_str;
  num->size = num->size + num_of_zeroes;

  return num;
}

/*static BigInt *_remove_leading_zeroes(BigInt *n) {

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
}*/

static inline int _less_or_equal_abs(const BigInt *lhs, const BigInt *rhs) {
  return (lhs->size > rhs->size) ? 0 : 1;
}

/*static BigInt *_calculate_sum(const BigInt *a, const BigInt *b, BigInt *res,
                              char res_sign) {

  int digit_diff = a->size - b->size;

  BigInt *smaller_abs_num =
      (digit_diff >= 0) ? bigint_create_copy(b) : bigint_create_copy(a);

  BigInt *greater_abs_num =
      (digit_diff >= 0) ? bigint_create_copy(a) : bigint_create_copy(b);

  if (!_add_leading_zeroes(smaller_abs_num, digit_diff))
    return NULL;

  char *sum_str = _alloc_cstr(a->size + 1);

  if (!sum_str)
    return NULL;

  int add_part = 0;
  int k = a->size;

  for (int i = greater_abs_num->size - 1; i >= 0; i--) {
    int digit_greater = greater_abs_num->cstr[i] - '0';
    int digit_smaller = smaller_abs_num->cstr[i] - '0';
    int sum = (digit_greater + digit_smaller + add_part);

    if (sum > 9) {
      sum -= 10;
      add_part = 1;
    } else {
      add_part = 0;
    }

    sum_str[k--] = sum + '0';
  }

  sum_str[k] = add_part ? '1' : '0';

  res->cstr = sum_str;
  res->size = a->size + 1;
  res->sign = res_sign;

  (diff > 0) ? remove_leading_zeroes(n2) : remove_leading_zeroes(n1);
  remove_leading_zeroes(res);

  return res;
}*/

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

/*static BigInt *difference(BigInt *n1, BigInt *n2, BigInt *res) {

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
}*/

BigInt *bigint_create() {

  BigInt *num = malloc(sizeof(BigInt));

  if (!num)
    return NULL;

  num->size = 0;
  num->sign = '+';
  num->cstr = NULL;
  num->capacity = BIGINT_START_CAPACITY;

  return num;
}

BigInt *bigint_create_copy(const BigInt *src_num) {

  if (!src_num)
    return NULL;

  BigInt *new_num = bigint_create();

  if (!new_num)
    return NULL;

  new_num->size = src_num->size;
  new_num->sign = src_num->sign;
  new_num->capacity = src_num->capacity;

  if (!src_num->cstr) {
    new_num->cstr = NULL;
    return new_num;
  }

  new_num->cstr = strdup(src_num->cstr);

  if (!new_num->cstr) {
    free(new_num);
    return NULL;
  }

  return new_num;
}

BigInt *bigint_create_from_num(long long src_num) {

  BigInt *bigint_num = bigint_create();

  if (!bigint_num)
    return NULL;

  bigint_num->sign = (src_num < 0) ? '-' : '+';
  bigint_num->size = _get_number_of_digits(src_num);
  bigint_num->capacity = bigint_num->size;

  char *alloc_cstr = _alloc_cstr(bigint_num->size);

  if(!alloc_cstr)
    return NULL;

  for(size_t i = 0; i < bigint_num->size; ++i){
    alloc_cstr[i] = (src_num % 10) + '0';
    src_num /= 10;
  }

  bigint_num->cstr = alloc_cstr;

  return bigint_num;
}

/*BigInt *bigint_create_from_cstr(const char *cstr) {

  if (!cstr)
    return NULL;

  BigInt *bigint_num = bigint_create();

  if (!bigint_num)
    return NULL;

  int create_status = 0;

  if (cstr[0] == '-' || cstr[0] == '+') {
    const char *magnitude = cstr + 1;
    create_status = _create_from_cstr(&bigint_num, magnitude, cstr[0]);
  } else {
    create_status = _create_from_cstr(&bigint_num, cstr, '+');
  }

  return (create_status == 1) ? bigint_num : free(bigint_num), NULL;
}*/

/*int bigint_less(const BigInt *lhs, const BigInt *rhs) {

  // NOTE: absent NULL check

  if (lhs->sign == '+' && rhs->sign == '-') {
    return 0;
  } else if (lhs->sign == '-' && rhs->sign == '+') {
    return 1;
  }

  int cmp = strcmp(lhs->cstr, rhs->cstr);

  if (lhs->sign == '+' && rhs->sign == '+') {
    return (cmp < 0) ? 1 : 0;
  }

  return (cmp < 0) ? 0 : 1;
}

int bigint_less_or_equal(const BigInt *lhs, const BigInt *rhs) {

  if (!bigint_is_equal(lhs, rhs)) {
    if (!bigint_less(lhs, rhs))
      return 0;
  }

  return 1;
}

int bigint_greater(const BigInt *lhs, const BigInt *rhs) {
  return !bigint_less(lhs, rhs);
}

int bigint_greater_or_equal(const BigInt *lhs, const BigInt *rhs) {

  if (!bigint_is_equal(lhs, rhs)) {
    if (bigint_less(lhs, rhs))
      return 0;
  }

  return 1;
}*/

/*int bigint_is_equal(const BigInt *lhs, const BigInt *rhs) {

  // NOTE: absent NULL check

  int is_equal_size = n1->size == n2->size;
  int is_equal_str = !strcmp(n1->ptr, n2->ptr);
  int is_equal_sign = n1->sign == n2->sign;

  if (is_equal_sign && is_equal_size && is_equal_str) {
    return 1;
  }

  return 0;
}

BigInt *bigint_sum(const BigInt *a, const BigInt *b) {

  if (!a || !b)
    return NULL;

  BigInt *res = bigint_create();

  if (!res)
    return NULL;

  if (a->sign == b->sign) {
    return _calculate_sum(a, b, res, a->sign);
  }

  return difference(a, b, res);
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
}*/

void bigint_negate(BigInt *bigint_num) {
  bigint_num->sign = (bigint_num->sign == '+') ? '-' : '+';
}

void bigint_free(BigInt *bigint_num) {
  free(bigint_num->cstr);
  free(bigint_num);
}
