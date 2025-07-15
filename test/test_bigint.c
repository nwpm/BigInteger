#include "../src/bigint.h"
#include "./Unity/src/unity.h"
#include "./Unity/src/unity_internals.h"
#include <stdlib.h>

void setUp() {}
void tearDown() {}

char *_create_and_fill_long_cstr(size_t size, unsigned char fill_by,
                                 unsigned char sign) {

  char *str = calloc(size + 2, sizeof(char));

  if (!str)
    return NULL;

  str[size + 1] = '\0';

  for (size_t i = 0; i < size + 1; ++i) {
    str[i] = fill_by;
  }

  str[0] = sign;

  return str;
}

int _check_is_correct_cstr(const char *source, const char *expected,
                           size_t size) {

  size_t j = 0;
  size_t i = size - 1;
  while (i < size) {
    if (expected[j++] != source[i--]) {
      return -1;
    }
  }

  return 0;
}

void test_bigint_create() {

  BigInt *bigint_num = bigint_create();

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(0, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(BIGINT_START_CAPACITY, bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('+', bigint_num->sign);
  TEST_ASSERT_NULL(bigint_num->cstr);

  bigint_free(bigint_num);
}

void test_bigint_create_from_num_zero() {

  BigInt *bigint_num = bigint_create_from_num(0);

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(1, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(BIGINT_START_CAPACITY, bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('+', bigint_num->sign);
  TEST_ASSERT_EQUAL_CHAR('0', bigint_num->cstr[0]);

  bigint_free(bigint_num);
}

void test_bigint_create_from_num_pos_less_100() {

  BigInt *bigint_num = bigint_create_from_num(10);

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(2, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(BIGINT_START_CAPACITY, bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('+', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(
      0, _check_is_correct_cstr(bigint_num->cstr, "10", bigint_num->size));

  bigint_free(bigint_num);
}

void test_bigint_create_from_num_pos_less_10000() {

  BigInt *bigint_num = bigint_create_from_num(9876);

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(4, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(bigint_num->size * 2, bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('+', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(
      0, _check_is_correct_cstr(bigint_num->cstr, "9876", bigint_num->size));

  bigint_free(bigint_num);
}

void test_bigint_create_from_num_pos_less_1e12() {

  BigInt *bigint_num = bigint_create_from_num(1e12 - 1);

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(12, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(bigint_num->size * 2, bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('+', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(0,
                        _check_is_correct_cstr(bigint_num->cstr, "999999999999",
                                               bigint_num->size));

  bigint_free(bigint_num);
}

void test_bigint_create_from_num_neg_less_100() {

  BigInt *bigint_num = bigint_create_from_num(-10);

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(2, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(BIGINT_START_CAPACITY, bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('-', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(
      0, _check_is_correct_cstr(bigint_num->cstr, "10", bigint_num->size));

  bigint_free(bigint_num);
}

void test_bigint_create_from_num_neg_less_10000() {

  BigInt *bigint_num = bigint_create_from_num(-9876);

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(4, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(bigint_num->size * 2, bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('-', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(
      0, _check_is_correct_cstr(bigint_num->cstr, "9876", bigint_num->size));

  bigint_free(bigint_num);
}

void test_bigint_create_from_num_neg_less_1e12() {

  BigInt *bigint_num = bigint_create_from_num(-1e12 + 1);

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(12, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(bigint_num->size * 2, bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('-', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(0,
                        _check_is_correct_cstr(bigint_num->cstr, "999999999999",
                                               bigint_num->size));

  bigint_free(bigint_num);
}

void test_bigint_create_from_cstr_pos_zero() {

  BigInt *bigint_num = bigint_create_from_cstr("0");

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(1, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(BIGINT_START_CAPACITY, bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('+', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(
      0, _check_is_correct_cstr(bigint_num->cstr, "0", bigint_num->size));

  bigint_free(bigint_num);
}

void test_bigint_create_from_cstr_neg_zero() {

  BigInt *bigint_num = bigint_create_from_cstr("-0");

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(1, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(BIGINT_START_CAPACITY, bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('-', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(
      0, _check_is_correct_cstr(bigint_num->cstr, "0", bigint_num->size));

  bigint_free(bigint_num);
}

void test_bigint_create_from_cstr_pos_size_2b() {

  BigInt *bigint_num = bigint_create_from_cstr("99");

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(2, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(BIGINT_START_CAPACITY, bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('+', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(
      0, _check_is_correct_cstr(bigint_num->cstr, "99", bigint_num->size));

  bigint_free(bigint_num);
}

void test_bigint_create_from_cstr_pos_size_6b() {

  BigInt *bigint_num = bigint_create_from_cstr("123456");

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(6, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(bigint_num->size * 2, bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('+', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(
      0, _check_is_correct_cstr(bigint_num->cstr, "123456", bigint_num->size));

  bigint_free(bigint_num);
}

void test_bigint_create_from_cstr_pos_size_64b() {

  char *source_cstr = _create_and_fill_long_cstr(64, '1', '+');

  TEST_ASSERT_NOT_NULL(source_cstr);

  BigInt *bigint_num = bigint_create_from_cstr(source_cstr);

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(64, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(bigint_num->size * 2, bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('+', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(0, _check_is_correct_cstr(bigint_num->cstr,
                                                  source_cstr + 1,
                                                  bigint_num->size));

  free(source_cstr);
  bigint_free(bigint_num);
}

void test_bigint_create_from_cstr_pos_size_1kb() {

  char *source_cstr = _create_and_fill_long_cstr(1000, '1', '+');

  TEST_ASSERT_NOT_NULL(source_cstr);

  BigInt *bigint_num = bigint_create_from_cstr(source_cstr);

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(1000, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(bigint_num->size + bigint_num->size / 2,
                           bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('+', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(0, _check_is_correct_cstr(bigint_num->cstr,
                                                  source_cstr + 1,
                                                  bigint_num->size));

  free(source_cstr);
  bigint_free(bigint_num);
}

void test_bigint_create_from_cstr_pos_size_100kb() {

  char *source_cstr = _create_and_fill_long_cstr(1e5, '1', '+');

  TEST_ASSERT_NOT_NULL(source_cstr);

  source_cstr[4] = '9';

  BigInt *bigint_num = bigint_create_from_cstr(source_cstr);

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(1e5, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(bigint_num->size + bigint_num->size / 10,
                           bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('+', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(0, _check_is_correct_cstr(bigint_num->cstr,
                                                  source_cstr + 1,
                                                  bigint_num->size));

  free(source_cstr);
  bigint_free(bigint_num);
}

void test_bigint_create_from_cstr_pos_size_100Mb() {

  char *source_cstr = _create_and_fill_long_cstr(1e8, '1', '+');

  TEST_ASSERT_NOT_NULL(source_cstr);

  source_cstr[4] = '9';

  BigInt *bigint_num = bigint_create_from_cstr(source_cstr);

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(1e8, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(bigint_num->size + bigint_num->size / 20,
                           bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('+', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(0, _check_is_correct_cstr(bigint_num->cstr,
                                                  source_cstr + 1,
                                                  bigint_num->size));

  free(source_cstr);
  bigint_free(bigint_num);
}

void test_bigint_create_from_cstr_pos_size_2Gb() {

  char *source_cstr = _create_and_fill_long_cstr(2e9, '1', '+');

  TEST_ASSERT_NOT_NULL(source_cstr);

  source_cstr[4] = '9';

  BigInt *bigint_num = bigint_create_from_cstr(source_cstr);

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(2e9, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(bigint_num->size + bigint_num->size / 20,
                           bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('+', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(0, _check_is_correct_cstr(bigint_num->cstr,
                                                  source_cstr + 1,
                                                  bigint_num->size));

  free(source_cstr);
  bigint_free(bigint_num);
}

void test_bigint_create_from_cstr_neg_size_2b() {

  BigInt *bigint_num = bigint_create_from_cstr("-99");

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(2, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(BIGINT_START_CAPACITY, bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('-', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(
      0, _check_is_correct_cstr(bigint_num->cstr, "99", bigint_num->size));

  bigint_free(bigint_num);
}

void test_bigint_create_from_cstr_neg_size_6b() {

  BigInt *bigint_num = bigint_create_from_cstr("-123456");

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(6, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(bigint_num->size * 2, bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('-', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(
      0, _check_is_correct_cstr(bigint_num->cstr, "123456", bigint_num->size));

  bigint_free(bigint_num);
}

void test_bigint_create_from_cstr_neg_size_64b() {

  char *source_cstr = _create_and_fill_long_cstr(64, '1', '-');

  TEST_ASSERT_NOT_NULL(source_cstr);

  BigInt *bigint_num = bigint_create_from_cstr(source_cstr);

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(64, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(bigint_num->size * 2, bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('-', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(0, _check_is_correct_cstr(bigint_num->cstr,
                                                  source_cstr + 1,
                                                  bigint_num->size));

  free(source_cstr);
  bigint_free(bigint_num);
}

void test_bigint_create_from_cstr_neg_size_1kb() {

  char *source_cstr = _create_and_fill_long_cstr(1000, '1', '-');

  TEST_ASSERT_NOT_NULL(source_cstr);

  source_cstr[4] = '9';

  BigInt *bigint_num = bigint_create_from_cstr(source_cstr);

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(1000, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(bigint_num->size + bigint_num->size / 2,
                           bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('-', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(0, _check_is_correct_cstr(bigint_num->cstr,
                                                  source_cstr + 1,
                                                  bigint_num->size));

  free(source_cstr);
  bigint_free(bigint_num);
}

void test_bigint_create_from_cstr_neg_size_100kb() {

  char *source_cstr = _create_and_fill_long_cstr(1e5, '1', '-');

  TEST_ASSERT_NOT_NULL(source_cstr);

  source_cstr[4] = '9';

  BigInt *bigint_num = bigint_create_from_cstr(source_cstr);

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(1e5, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(bigint_num->size + bigint_num->size / 10,
                           bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('-', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(0, _check_is_correct_cstr(bigint_num->cstr,
                                                  source_cstr + 1,
                                                  bigint_num->size));

  free(source_cstr);
  bigint_free(bigint_num);
}

void test_bigint_create_from_cstr_neg_size_100Mb() {

  char *source_cstr = _create_and_fill_long_cstr(1e8, '1', '-');

  TEST_ASSERT_NOT_NULL(source_cstr);

  source_cstr[4] = '9';

  BigInt *bigint_num = bigint_create_from_cstr(source_cstr);

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(1e8, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(bigint_num->size + bigint_num->size / 20,
                           bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('-', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(0, _check_is_correct_cstr(bigint_num->cstr,
                                                  source_cstr + 1,
                                                  bigint_num->size));

  free(source_cstr);
  bigint_free(bigint_num);
}

void test_bigint_create_from_cstr_neg_size_2Gb() {

  char *source_cstr = _create_and_fill_long_cstr(2e9, '1', '-');

  TEST_ASSERT_NOT_NULL(source_cstr);

  source_cstr[4] = '9';

  BigInt *bigint_num = bigint_create_from_cstr(source_cstr);

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(2e9, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(bigint_num->size + bigint_num->size / 20,
                           bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('-', bigint_num->sign);

  TEST_ASSERT_EQUAL_INT(0, _check_is_correct_cstr(bigint_num->cstr,
                                                  source_cstr + 1,
                                                  bigint_num->size));

  free(source_cstr);
  bigint_free(bigint_num);
}

void test_bigint_create_from_cstr_NULL() {

  BigInt *bigint_num = bigint_create_from_cstr(NULL);

  TEST_ASSERT_NULL(bigint_num);
}

void test_bigint_create_from_copy_empty() {

  BigInt *bigint_num = bigint_create();

  TEST_ASSERT_NOT_NULL(bigint_num);

  BigInt *bigint_copy = bigint_create_copy(bigint_num);

  TEST_ASSERT_NOT_NULL(bigint_copy);

  TEST_ASSERT_EQUAL_size_t(0, bigint_copy->size);
  TEST_ASSERT_EQUAL_size_t(BIGINT_START_CAPACITY, bigint_copy->capacity);
  TEST_ASSERT_EQUAL_CHAR('+', bigint_copy->sign);
  TEST_ASSERT_NULL(bigint_copy->cstr);

  bigint_free(bigint_num);
  bigint_free(bigint_copy);
}

void test_bigint_create_from_copy_zero() {

  BigInt *bigint_num = bigint_create_from_num(0);

  TEST_ASSERT_NOT_NULL(bigint_num);

  BigInt *bigint_copy = bigint_create_copy(bigint_num);

  TEST_ASSERT_NOT_NULL(bigint_copy);

  TEST_ASSERT_EQUAL_size_t(1, bigint_copy->size);
  TEST_ASSERT_EQUAL_size_t(BIGINT_START_CAPACITY, bigint_copy->capacity);
  TEST_ASSERT_EQUAL_CHAR('+', bigint_copy->sign);
  TEST_ASSERT_EQUAL_CHAR('0', bigint_num->cstr[0]);

  bigint_free(bigint_num);
  bigint_free(bigint_copy);
}

void test_bigint_create_from_copy_num_pos_less_1000() {

  BigInt *bigint_num = bigint_create_from_num(987);

  TEST_ASSERT_NOT_NULL(bigint_num);

  BigInt *bigint_copy = bigint_create_copy(bigint_num);

  TEST_ASSERT_NOT_NULL(bigint_copy);

  TEST_ASSERT_EQUAL_size_t(3, bigint_copy->size);
  TEST_ASSERT_EQUAL_size_t(BIGINT_START_CAPACITY, bigint_copy->capacity);
  TEST_ASSERT_EQUAL_CHAR('+', bigint_copy->sign);

  TEST_ASSERT_EQUAL_INT(
      0, _check_is_correct_cstr(bigint_num->cstr, "987", bigint_num->size));

  bigint_free(bigint_num);
  bigint_free(bigint_copy);
}

void test_bigint_create_from_copy_num_neg_less_1000() {

  BigInt *bigint_num = bigint_create_from_num(-987);

  TEST_ASSERT_NOT_NULL(bigint_num);

  BigInt *bigint_copy = bigint_create_copy(bigint_num);

  TEST_ASSERT_NOT_NULL(bigint_copy);

  TEST_ASSERT_EQUAL_size_t(3, bigint_copy->size);
  TEST_ASSERT_EQUAL_size_t(BIGINT_START_CAPACITY, bigint_copy->capacity);
  TEST_ASSERT_EQUAL_CHAR('-', bigint_copy->sign);

  TEST_ASSERT_EQUAL_INT(
      0, _check_is_correct_cstr(bigint_num->cstr, "987", bigint_num->size));

  bigint_free(bigint_num);
  bigint_free(bigint_copy);
}

void test_bigint_create_from_copy_num_pos_10Mb() {

  BigInt *bigint_num = bigint_create_from_cstr("10000000");

  TEST_ASSERT_NOT_NULL(bigint_num);

  BigInt *bigint_copy = bigint_create_copy(bigint_num);

  TEST_ASSERT_NOT_NULL(bigint_copy);

  TEST_ASSERT_EQUAL_size_t(8, bigint_copy->size);
  TEST_ASSERT_EQUAL_size_t(bigint_copy->size * 2, bigint_copy->capacity);
  TEST_ASSERT_EQUAL_CHAR('+', bigint_copy->sign);

  TEST_ASSERT_EQUAL_INT(0, _check_is_correct_cstr(bigint_num->cstr, "10000000",
                                                  bigint_num->size));

  bigint_free(bigint_num);
  bigint_free(bigint_copy);
}

void test_bigint_create_from_copy_num_neg_10Mb() {

  BigInt *bigint_num = bigint_create_from_cstr("-10000000");

  TEST_ASSERT_NOT_NULL(bigint_num);

  BigInt *bigint_copy = bigint_create_copy(bigint_num);

  TEST_ASSERT_NOT_NULL(bigint_copy);

  TEST_ASSERT_EQUAL_size_t(8, bigint_copy->size);
  TEST_ASSERT_EQUAL_size_t(bigint_copy->size * 2, bigint_copy->capacity);
  TEST_ASSERT_EQUAL_CHAR('-', bigint_copy->sign);

  TEST_ASSERT_EQUAL_INT(0, _check_is_correct_cstr(bigint_num->cstr, "10000000",
                                                  bigint_num->size));

  bigint_free(bigint_num);
  bigint_free(bigint_copy);
}

void test_bigint_create_from_copy_NULL() {

  BigInt *bigint_num = bigint_create_copy(NULL);

  TEST_ASSERT_NULL(bigint_num);
}

int main() {

  UNITY_BEGIN();

  printf("\n");

  RUN_TEST(test_bigint_create);

  printf("\n");

  RUN_TEST(test_bigint_create_from_num_zero);
  RUN_TEST(test_bigint_create_from_num_pos_less_100);
  RUN_TEST(test_bigint_create_from_num_pos_less_10000);
  RUN_TEST(test_bigint_create_from_num_pos_less_1e12);
  RUN_TEST(test_bigint_create_from_num_neg_less_100);
  RUN_TEST(test_bigint_create_from_num_neg_less_10000);
  RUN_TEST(test_bigint_create_from_num_neg_less_1e12);

  printf("\n");

  RUN_TEST(test_bigint_create_from_cstr_pos_zero);
  RUN_TEST(test_bigint_create_from_cstr_neg_zero);

  printf("\n");

  RUN_TEST(test_bigint_create_from_cstr_pos_size_2b);
  RUN_TEST(test_bigint_create_from_cstr_pos_size_6b);
  RUN_TEST(test_bigint_create_from_cstr_pos_size_64b);
  RUN_TEST(test_bigint_create_from_cstr_pos_size_100kb);
  // RUN_TEST(test_bigint_create_from_cstr_pos_size_100Mb);
  // RUN_TEST(test_bigint_create_from_cstr_pos_size_2Gb);

  printf("\n");

  RUN_TEST(test_bigint_create_from_cstr_neg_size_2b);
  RUN_TEST(test_bigint_create_from_cstr_neg_size_6b);
  RUN_TEST(test_bigint_create_from_cstr_neg_size_64b);
  RUN_TEST(test_bigint_create_from_cstr_neg_size_100kb);
  // RUN_TEST(test_bigint_create_from_cstr_neg_size_100Mb);
  // RUN_TEST(test_bigint_create_from_cstr_neg_size_2Gb);

  RUN_TEST(test_bigint_create_from_cstr_NULL);

  printf("\n");

  RUN_TEST(test_bigint_create_from_copy_NULL);
  RUN_TEST(test_bigint_create_from_copy_empty);
  RUN_TEST(test_bigint_create_from_copy_zero);
  RUN_TEST(test_bigint_create_from_copy_num_pos_less_1000);
  RUN_TEST(test_bigint_create_from_copy_num_neg_less_1000);
  RUN_TEST(test_bigint_create_from_copy_num_pos_10Mb);
  RUN_TEST(test_bigint_create_from_copy_num_neg_10Mb);

  return UNITY_END();
}
