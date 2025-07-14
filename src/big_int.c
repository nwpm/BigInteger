#include "big_int.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *_alloc_cstr(size_t size) {

  char *arr_c = malloc(size);

  if (!arr_c)
    return NULL;

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

size_t _calculate_capacity(size_t size) {
  if (size < 100) {
    return size * 2;
  } else if (size < 10000) {
    return size + size / 2;
  } else if (size < 1000000) {
    return size + size / 10;
  }
  return size + (size / 20);
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
  n->capacity = _calculate_capacity(cstr_len);

  char *alloc_cstr = _alloc_cstr(n->capacity);

  if (!alloc_cstr)
    return 0;

  size_t j = 0;
  size_t i = cstr_len;
  while (i-- > 0) {
    alloc_cstr[j++] = cstr[i];
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

static int _abs_compare(const BigInt *lhs, const BigInt *rhs) {

  if (lhs->size != rhs->size) {
    return (lhs->size > rhs->size) ? 1 : -1;
  }

  for (size_t i = lhs->size; i > 0; i--) {
    if (lhs->cstr[i - 1] > rhs->cstr[i - 1])
      return 1;
    if (lhs->cstr[i - 1] < rhs->cstr[i - 1])
      return -1;
  }

  return 0;
}

static int _calculate_abs_sum(BigInt *target, const BigInt *source, char res_sign) {

  int compare_res = _abs_compare(target, source);

  const BigInt *smaller_abs_num = (compare_res >= 0) ? source : target;
  const BigInt *greater_abs_num = (compare_res >= 0) ? target : source;

  size_t max_size = greater_abs_num->size + 1;

  if (max_size >= target->capacity) {
    void *new_cstr = realloc(target->cstr, max_size);

    if (!new_cstr) {
      return -1;
    }

    target->cstr = new_cstr;
  }

  int add_part = 0;
  size_t i = 0;

  while (i < smaller_abs_num->size) {
    int digit_greater = greater_abs_num->cstr[i] - '0';
    int digit_smaller = smaller_abs_num->cstr[i] - '0';
    int sum = (digit_greater + digit_smaller + add_part);

    if (sum > 9) {
      sum -= 10;
      add_part = 1;
    } else {
      add_part = 0;
    }

    target->cstr[i++] = sum + '0';
  }

  while (i < greater_abs_num->size) {
    int digit_greater = greater_abs_num->cstr[i] - '0';
    int sum = digit_greater + add_part;

    if (sum > 9) {
      sum -= 10;
      add_part = 1;
    } else {
      add_part = 0;
    }

    target->cstr[i++] = sum + '0';
  }

  if (add_part) {
    target->cstr[i++] = add_part + '0';
  }

  target->size = i;
  target->sign = res_sign;

  return 0;
}

static int _calculate_abs_dif(BigInt *target, const BigInt *source) {

  int compare_res = _abs_compare(target, source);

  const BigInt *smaller_abs_num = (compare_res >= 0) ? source : target;
  const BigInt *greater_abs_num = (compare_res >= 0) ? target : source;

  int substr_part = 0;
  size_t i = 0;

  while (i < smaller_abs_num->size) {
    int digit_greater = greater_abs_num->cstr[i] - '0';
    int digit_smaller = smaller_abs_num->cstr[i] - '0';
    int substr = (digit_greater - digit_smaller) - substr_part;

    if (substr < 0) {
      substr += 10;
      substr_part = 1;
    } else {
      substr_part = 0;
    }
    target->cstr[i++] = substr + '0';
  }

  while (substr_part) {
    int digit_greater = greater_abs_num->cstr[i] - '0';
    int substr = digit_greater - substr_part;

    if (substr < 0) {
      substr += 10;
      substr_part = 1;
    } else {
      substr_part = 0;
    }
    target->cstr[i++] = substr + '0';
  }

  target->size = i;

  while (target->size > 1 && target->cstr[target->size - 1] == '0') {
    target->size--;
  }

  target->sign = greater_abs_num->sign;

  return 0;
}

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
  bigint_num->capacity = _calculate_capacity(bigint_num->size);

  char *alloc_cstr = _alloc_cstr(bigint_num->capacity);

  if (!alloc_cstr)
    return NULL;

  for (size_t i = 0; i < bigint_num->size; ++i) {
    alloc_cstr[i] = (src_num % 10) + '0';
    src_num /= 10;
  }

  bigint_num->cstr = alloc_cstr;

  return bigint_num;
}

BigInt *bigint_create_from_cstr(const char *cstr) {

  if (!cstr)
    return NULL;

  BigInt *bigint_num = bigint_create();

  if (!bigint_num)
    return NULL;

  int create_status = 0;

  if (cstr[0] == '-' || cstr[0] == '+') {
    const char *magnitude = cstr + 1;
    create_status = _create_from_cstr(bigint_num, magnitude, cstr[0]);
  } else {
    create_status = _create_from_cstr(bigint_num, cstr, '+');
  }

  return (create_status == 1) ? bigint_num : free(bigint_num), NULL;
}

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
}*/

BigInt *bigint_assign(BigInt *target, const BigInt *source) {

  if (!target || !source)
    return NULL;

  int assign_status = 0;

  if (target->sign == source->sign) {
    assign_status = _calculate_abs_sum(target, source, target->sign);
  } else {
    assign_status = _calculate_abs_dif(target, source);
  }

  return (assign_status == 0) ? target : NULL;
}

BigInt *bigint_substract(BigInt *target, const BigInt *source) {

  if (!target || !source)
    return NULL;

  if (target->sign == source->sign) {
    _calculate_abs_dif(target, source);
  } else if (target->sign == '+' && source->sign == '-') {
    _calculate_abs_sum(target, source, '+');
  } else if (target->sign == '-' && source->sign == '+') {
    _calculate_abs_sum(target, source, '-');
  }

  return target;
}

BigInt *bigint_abs(BigInt *bigint_num) {

  if (!bigint_num)
    return NULL;

  bigint_num->sign = '+';

  return bigint_num;
}

BigInt *bigint_negate(BigInt *bigint_num) {

  if (!bigint_num)
    return NULL;

  bigint_num->sign = (bigint_num->sign == '+') ? '-' : '+';

  return bigint_num;
}

void bigint_free(BigInt *bigint_num) {
  free(bigint_num->cstr);
  free(bigint_num);
}
