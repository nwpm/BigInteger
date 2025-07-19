#include "bigint.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static char *_alloc_buffer(size_t size) {

  char *buf = malloc(size);

  if (!buf)
    return NULL;

  return buf;
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

static size_t _calculate_capacity(size_t size) {
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

static int _ensure_capacity(BigInt *bigint_num, size_t max_size) {

  if (bigint_num->capacity < max_size) {

    bigint_num->capacity = _calculate_capacity(max_size);

    void *new_buffer = realloc(bigint_num->buffer, bigint_num->capacity);

    if (!new_buffer) {
      return -1;
    }

    bigint_num->buffer = new_buffer;
  }
  return 0;
}

static int _buff_cmp(const BigInt *a, const BigInt *b) {

  if (a->size == 0) {
    return 1;
  }

  for (size_t i = a->size; i > 0; --i) {
    if (a->buffer[i - 1] != b->buffer[i - 1]) {
      return 0;
    }
  }

  return 1;
}

static int _create_from_cstr(BigInt *n, const char *cstr, char sign) {

  if (!_is_valid_cstr(cstr)) {
    return -1;
  }

  size_t cstr_len = strlen(cstr);

  if (cstr_len >= n->capacity) {
    n->capacity = _calculate_capacity(cstr_len);
  }

  char *alloc_buffer = _alloc_buffer(n->capacity);

  if (!alloc_buffer)
    return -1;

  size_t j = 0;
  size_t i = cstr_len;
  while (i-- > 0) {
    alloc_buffer[j++] = cstr[i];
  }

  n->buffer = alloc_buffer;
  n->size = cstr_len;
  n->sign = sign;

  return 0;
}

static char *_buff_dup(const BigInt *num) {

  char *copy_buff = malloc(num->capacity);

  if (!copy_buff)
    return NULL;

  memcpy(copy_buff, num->buffer, num->size);

  return copy_buff;
}

static int _abs_compare(const BigInt *lhs, const BigInt *rhs) {

  if (lhs->size != rhs->size) {
    return (lhs->size > rhs->size) ? 1 : -1;
  }

  for (size_t i = lhs->size; i > 0; i--) {
    if (lhs->buffer[i - 1] > rhs->buffer[i - 1])
      return 1;
    if (lhs->buffer[i - 1] < rhs->buffer[i - 1])
      return -1;
  }

  return 0;
}

static char _calculate_res_sign(const BigInt *lhs, const BigInt *rhs,
                                char operation) {

  int compare_res = _abs_compare(lhs, rhs);

  if (operation == '+') {
    if (compare_res == 0) {
      return (lhs->sign == '-' && rhs->sign == '-') ? '-' : '+';
    } else if (compare_res == 1) {
      return (lhs->sign == '-') ? '-' : '+';
    } else if (compare_res == -1) {
      return (rhs->sign == '+') ? '+' : '-';
    }
  }

  if (compare_res == 0) {
    return (lhs->sign == '-' && rhs->sign == '+') ? '-' : '+';
  } else if (compare_res == 1) {
    return (lhs->sign == '-') ? '-' : '+';
  } else if (compare_res == -1) {
    return (rhs->sign == '+') ? '-' : '+';
  }

  return '#';
}

static int _calculate_abs_sum(BigInt *bigint_num, const BigInt *addend,
                              char res_sign) {

  int compare_res = _abs_compare(bigint_num, addend);

  const BigInt *smaller_abs_num = (compare_res >= 0) ? addend : bigint_num;
  const BigInt *greater_abs_num = (compare_res >= 0) ? bigint_num : addend;

  size_t max_size = greater_abs_num->size + 1;

  if(_ensure_capacity(bigint_num, max_size) != 0)
    return -1;

  int add_part = 0;
  size_t i = 0;

  while (i < smaller_abs_num->size) {
    int digit_greater = greater_abs_num->buffer[i] - '0';
    int digit_smaller = smaller_abs_num->buffer[i] - '0';
    int sum = (digit_greater + digit_smaller + add_part);

    if (sum > 9) {
      sum -= 10;
      add_part = 1;
    } else {
      add_part = 0;
    }

    bigint_num->buffer[i++] = sum + '0';
  }

  while (i < greater_abs_num->size) {
    int digit_greater = greater_abs_num->buffer[i] - '0';
    int sum = digit_greater + add_part;

    if (sum > 9) {
      sum -= 10;
      add_part = 1;
    } else {
      add_part = 0;
    }

    bigint_num->buffer[i++] = sum + '0';
  }

  if (add_part) {
    bigint_num->buffer[i++] = add_part + '0';
  }

  bigint_num->size = i;
  bigint_num->sign = res_sign;

  return 0;
}

static int _calculate_abs_dif(BigInt *bigint_num, const BigInt *substr,
                              char res_sign) {

  int compare_res = _abs_compare(bigint_num, substr);

  const BigInt *smaller_abs_num = (compare_res >= 0) ? substr : bigint_num;
  const BigInt *greater_abs_num = (compare_res >= 0) ? bigint_num : substr;

  if (bigint_num->capacity < greater_abs_num->size) {

    bigint_num->capacity = _calculate_capacity(greater_abs_num->size);
    void *new_buffer = realloc(bigint_num->buffer, greater_abs_num->capacity);

    if (!new_buffer) {
      return -1;
    }

    bigint_num->buffer = new_buffer;
  }

  int substr_part = 0;
  size_t i = 0;

  while (i < smaller_abs_num->size) {
    int digit_greater = greater_abs_num->buffer[i] - '0';
    int digit_smaller = smaller_abs_num->buffer[i] - '0';
    int substr = (digit_greater - digit_smaller) - substr_part;

    if (substr < 0) {
      substr += 10;
      substr_part = 1;
    } else {
      substr_part = 0;
    }
    bigint_num->buffer[i++] = substr + '0';
  }

  while (i < greater_abs_num->size) {
    int digit_greater = greater_abs_num->buffer[i] - '0';
    int substr = digit_greater - substr_part;

    if (substr < 0) {
      substr += 10;
      substr_part = 1;
    } else {
      substr_part = 0;
    }
    bigint_num->buffer[i++] = substr + '0';
  }

  bigint_num->size = i;

  while (bigint_num->size > 1 &&
         bigint_num->buffer[bigint_num->size - 1] == '0') {
    bigint_num->size--;
  }

  bigint_num->sign = res_sign;

  return 0;
}

static int _calculate_abs_mult(BigInt *bigint_num, const BigInt *multiplyer) {

  int compare_res = _abs_compare(bigint_num, multiplyer);

  const BigInt *smaller_abs_num = (compare_res >= 0) ? multiplyer : bigint_num;
  const BigInt *greater_abs_num = (compare_res >= 0) ? bigint_num : multiplyer;

  size_t max_size = greater_abs_num->size + smaller_abs_num->size;

  BigInt *res_of_mult = bigint_create_from_num(0);

  if (!res_of_mult)
    return -1;

  BigInt *temp = bigint_create_from_num(0);

  if (!temp)
    return -1;

  if(_ensure_capacity(temp, max_size) != 0)
    return -1;

  for (size_t i = 0; i < smaller_abs_num->size; ++i) {
    int digit_smaller = smaller_abs_num->buffer[i] - '0';
    int mult_part = 0;
    size_t k = 0;

    for (; k < i; ++k) {
      temp->buffer[k] = 0 + '0';
    }

    for (size_t j = 0; j < greater_abs_num->size; ++j) {
      int digit_greater = greater_abs_num->buffer[j] - '0';

      int mult = (digit_smaller * digit_greater) + mult_part;

      if (mult > 10) {
        mult_part = mult / 10;
        mult %= 10;
      } else {
        mult_part = 0;
      }
      temp->buffer[k++] = mult + '0';
    }
    if (mult_part) {
      temp->buffer[k++] = mult_part + '0';
    }

    temp->size = k;

    bigint_add(res_of_mult, temp);
  }

  res_of_mult->sign = (bigint_num->sign == multiplyer->sign) ? '+' : '-';

  free(bigint_num->buffer);
  bigint_num->buffer = res_of_mult->buffer;
  bigint_num->size = res_of_mult->size;
  bigint_num->sign = res_of_mult->sign;
  bigint_num->capacity = res_of_mult->capacity;
  free(res_of_mult);

  bigint_free(temp);

  return 0;
}

BigInt *bigint_create() {

  BigInt *bigint_num = malloc(sizeof(BigInt));

  if (!bigint_num)
    return NULL;

  bigint_num->size = 0;
  bigint_num->sign = '+';
  bigint_num->buffer = NULL;
  bigint_num->capacity = BIGINT_START_CAPACITY;

  return bigint_num;
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

  if (!src_num->buffer) {
    new_num->buffer = NULL;
    return new_num;
  }

  new_num->buffer = _buff_dup(src_num);

  if (!new_num->buffer) {
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

  if (bigint_num->size >= BIGINT_START_CAPACITY) {
    bigint_num->capacity = _calculate_capacity(bigint_num->size);
  }

  char *alloc_buffer = _alloc_buffer(bigint_num->capacity);

  if (!alloc_buffer)
    return NULL;

  long long abs_num = _abs(src_num);

  for (size_t i = 0; i < bigint_num->size; ++i) {
    alloc_buffer[i] = (abs_num % 10) + '0';
    abs_num /= 10;
  }

  bigint_num->buffer = alloc_buffer;

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

  if (create_status != 0) {
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

  int is_negative = (lhs->sign == '-');
  if (lhs->size != rhs->size) {
    return is_negative ? (lhs->size > rhs->size) : (lhs->size < rhs->size);
  }

  for (size_t i = lhs->size; i > 0; i--) {
    if (lhs->buffer[i - 1] != rhs->buffer[i - 1]) {
      return is_negative ? (lhs->buffer[i - 1] > rhs->buffer[i - 1])
                         : (lhs->buffer[i - 1] < rhs->buffer[i - 1]);
    }
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
  return !bigint_less_or_equal(lhs, rhs);
}

int bigint_greater_or_equal(const BigInt *lhs, const BigInt *rhs) {

  if (!bigint_is_equal(lhs, rhs)) {
    if (bigint_less(lhs, rhs))
      return 0;
  }

  return 1;
}

int bigint_is_equal(const BigInt *lhs, const BigInt *rhs) {

  if (lhs->size == rhs->size) {
    if (lhs->sign == rhs->sign) {
      if (_buff_cmp(lhs, rhs))
        return 1;
    }
  }

  return 0;
}

BigInt *bigint_add(BigInt *bigint_num, const BigInt *addend) {

  if (!bigint_num || !addend)
    return NULL;

  int add_status = 0;
  char res_sign = _calculate_res_sign(bigint_num, addend, '+');

  if (bigint_num->sign == addend->sign) {
    add_status = _calculate_abs_sum(bigint_num, addend, bigint_num->sign);
  } else {
    add_status = _calculate_abs_dif(bigint_num, addend, res_sign);
  }

  return (add_status == 0) ? bigint_num : NULL;
}

BigInt *bigint_subtract(BigInt *bigint_num, const BigInt *substr) {

  if (!bigint_num || !substr)
    return NULL;

  int substr_status = 0;
  char res_sign = _calculate_res_sign(bigint_num, substr, '-');

  if (bigint_num->sign == substr->sign) {
    substr_status = _calculate_abs_dif(bigint_num, substr, res_sign);
  } else {
    substr_status = _calculate_abs_sum(bigint_num, substr, res_sign);
  }

  return (substr_status == 0) ? bigint_num : NULL;
}

BigInt *bigint_multiply(BigInt *bigint_num, const BigInt *multiplier) {

  if (!bigint_num || !multiplier)
    return NULL;

  int res_multiply = _calculate_abs_mult(bigint_num, multiplier);

  return (res_multiply == 0) ? bigint_num : NULL;
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
  free(bigint_num->buffer);
  free(bigint_num);
}
