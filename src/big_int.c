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

  if (num >= 0 && num < 10)
    return 1;

  long long result = 1;

  while (num /= 10)
    result++;

  return result;
}

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

  if(create_status != 1){
    free(bigint_num);
    return NULL;
  }

  return bigint_num;
}

int bigint_less(const BigInt *lhs, const BigInt *rhs) {

  if (lhs->sign == '+' && rhs->sign == '-') {
    return 0;
  } else if (lhs->sign == '-' && rhs->sign == '+') {
    return 1;
  }

  for (size_t i = lhs->size; i > 0; i--) {
    if (lhs->cstr[i - 1] > rhs->cstr[i - 1])
      return 0;
    if (lhs->cstr[i - 1] < rhs->cstr[i - 1])
      return 1;
  }

  return 0;
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
}

static int _cstr_cmp(const BigInt* a, const BigInt *b){

  if(a->size == 0){
    return 1;
  }

  for(size_t i = a->size; i > 0; --i){
    if(a->cstr[i - 1] != b->cstr[i - 1]){
      return 0;
    }
  }

  return 1;
}

int bigint_is_equal(const BigInt *lhs, const BigInt *rhs) {

  if(lhs->size == rhs->size){
    if(lhs->sign == rhs->sign){
      if(_cstr_cmp(lhs, rhs)){
        return 1;
      }
    }
  }

  return 0;
}

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
