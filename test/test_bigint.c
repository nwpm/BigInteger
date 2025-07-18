#include "../src/bigint.h"
#include "./Unity/src/unity.h"
#include "./Unity/src/unity_internals.h"
#include <stdio.h>
#include <stdlib.h>

void setUp() {}
void tearDown() {}

char *_create_and_fill_long_buffer(size_t size, unsigned char fill_by,
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

int _check_is_correct_buffer(const char *source, const char *expected,
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

void test_create() {

  BigInt *bigint_num = bigint_create();

  TEST_ASSERT_NOT_NULL(bigint_num);

  TEST_ASSERT_EQUAL_size_t(0, bigint_num->size);
  TEST_ASSERT_EQUAL_size_t(BIGINT_START_CAPACITY, bigint_num->capacity);
  TEST_ASSERT_EQUAL_CHAR('+', bigint_num->sign);
  TEST_ASSERT_NULL(bigint_num->buffer);

  bigint_free(bigint_num);
}

typedef struct TestCreateFromNum {
  long long input_num;
  size_t expected_size;
  size_t expected_capacity;
  char expected_sign;
  const char *expected_buffer;
} TestCreateFromNum;

TestCreateFromNum create_from_num_cases[] = {
    {0, 1, 4, '+', "0"},
    {10, 2, 4, '+', "10"},
    {9876, 4, 8, '+', "9876"},
    {1e12 - 1, 12, 24, '+', "999999999999"},
    {-10, 2, 4, '-', "10"},
    {-9876, 4, 8, '-', "9876"},
    {-1e12 + 1, 12, 24, '-', "999999999999"},
};

void tests_create_from_num() {
  size_t num_of_tests =
      sizeof(create_from_num_cases) / sizeof(create_from_num_cases[0]);
  for (size_t i = 0; i < num_of_tests; ++i) {

    char message[256];
    sprintf(message, "Case %zu: Input num %lld", i,
            create_from_num_cases[i].input_num);

    BigInt *bigint_num =
        bigint_create_from_num(create_from_num_cases[i].input_num);

    TEST_ASSERT_NOT_NULL_MESSAGE(bigint_num, message);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(create_from_num_cases[i].expected_size,
                                     bigint_num->size, message);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(create_from_num_cases[i].expected_capacity,
                                     bigint_num->capacity, message);
    TEST_ASSERT_EQUAL_CHAR_MESSAGE(create_from_num_cases[i].expected_sign,
                                   bigint_num->sign, message);

    TEST_ASSERT_EQUAL_INT_MESSAGE(
        0,
        _check_is_correct_buffer(bigint_num->buffer,
                                 create_from_num_cases[i].expected_buffer,
                                 bigint_num->size),
        message);

    bigint_free(bigint_num);
  }
}

typedef struct TestCreateFromCstr {
  const char *input_cstr;
  size_t expected_size;
  size_t expected_capacity;
  char expected_sign;
  const char *expected_buffer;
} TestCreateFromCstr;

TestCreateFromCstr create_from_cstr_cases[] = {
    {"0", 1, 4, '+', "0"},
    {"-0", 1, 4, '-', "0"},
    {"99", 2, 4, '+', "99"},
    {"-123456", 6, 12, '-', "123456"},
    {"999999999999", 12, 24, '+', "999999999999"},
    {"-999999999999", 12, 24, '-', "999999999999"},
};

void tests_create_from_cstr() {
  size_t num_of_tests =
      sizeof(create_from_cstr_cases) / sizeof(create_from_cstr_cases[0]);
  for (size_t i = 0; i < num_of_tests; ++i) {

    char message[256];
    sprintf(message, "Case %zu: Input cstr %s", i,
            create_from_cstr_cases[i].input_cstr);

    BigInt *bigint_num =
        bigint_create_from_cstr(create_from_cstr_cases[i].input_cstr);

    TEST_ASSERT_NOT_NULL_MESSAGE(bigint_num, message);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(create_from_cstr_cases[i].expected_size,
                                     bigint_num->size, message);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(
        create_from_cstr_cases[i].expected_capacity, bigint_num->capacity,
        message);
    TEST_ASSERT_EQUAL_CHAR_MESSAGE(create_from_cstr_cases[i].expected_sign,
                                   bigint_num->sign, message);

    TEST_ASSERT_EQUAL_INT_MESSAGE(
        0,
        _check_is_correct_buffer(bigint_num->buffer,
                                 create_from_cstr_cases[i].expected_buffer,
                                 bigint_num->size),
        message);

    bigint_free(bigint_num);
  }
}

typedef struct TestCreateCopy {

  const char *input_cstr;
  size_t expected_size;
  size_t expected_capacity;
  char expected_sign;
  const char *expected_buffer;
} TestCreateCopy;

TestCreateCopy create_copy_cases[] = {
    {"", 0, 4, '+', NULL},
    {"0", 1, 4, '+', "0"},
    {"987", 3, 4, '+', "987"},
    {"-987", 3, 4, '-', "987"},
    {"-123456", 6, 12, '-', "123456"},
    {"999999999999", 12, 24, '+', "999999999999"},
    {"-999999999999", 12, 24, '-', "999999999999"},
};

void tests_create_copy() {
  size_t num_of_tests =
      sizeof(create_copy_cases) / sizeof(create_copy_cases[0]);
  for (size_t i = 0; i < num_of_tests; ++i) {

    char message[256];
    sprintf(message, "Case %zu: Input cstr %s", i,
            create_copy_cases[i].input_cstr);

    BigInt *source = bigint_create_from_cstr(create_copy_cases[i].input_cstr);

    TEST_ASSERT_NOT_NULL_MESSAGE(source, message);

    BigInt *bigint_num = bigint_create_copy(source);

    TEST_ASSERT_NOT_NULL_MESSAGE(bigint_num, message);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(create_copy_cases[i].expected_size,
                                     bigint_num->size, message);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(create_copy_cases[i].expected_capacity,
                                     bigint_num->capacity, message);
    TEST_ASSERT_EQUAL_CHAR_MESSAGE(create_copy_cases[i].expected_sign,
                                   bigint_num->sign, message);

    TEST_ASSERT_EQUAL_INT_MESSAGE(
        0,
        _check_is_correct_buffer(bigint_num->buffer,
                                 create_copy_cases[i].expected_buffer,
                                 bigint_num->size),
        message);

    bigint_free(bigint_num);
    bigint_free(source);
  }
}

typedef struct TestLess {

  const char *lhs_cstr;
  const char *rhs_cstr;
  int expected_res;
} TestLess;

TestLess less_cases[] = {
    {"0", "0", 0},    {"10", "10", 0},  {"-10", "-10", 0},
    {"1", "-1", 0},   {"-1", "1", 1},   {"1234", "12", 0},
    {"-12", "-1", 1}, {"0", "1000", 1}, {"-1000", "0", 1},
};

void tests_less() {
  size_t num_of_tests = sizeof(less_cases) / sizeof(less_cases[0]);
  for (size_t i = 0; i < num_of_tests; ++i) {

    char message[256];
    sprintf(message, "Case %zu: Input lhs %s, input rhs %s", i,
            less_cases[i].lhs_cstr, less_cases[i].rhs_cstr);

    BigInt *lhs = bigint_create_from_cstr(less_cases[i].lhs_cstr);

    TEST_ASSERT_NOT_NULL_MESSAGE(lhs, message);

    BigInt *rhs = bigint_create_from_cstr(less_cases[i].rhs_cstr);

    TEST_ASSERT_NOT_NULL_MESSAGE(rhs, message);

    TEST_ASSERT_EQUAL_INT_MESSAGE(less_cases[i].expected_res,
                                  bigint_less(lhs, rhs), message);

    bigint_free(lhs);
    bigint_free(rhs);
  }
}

typedef struct TestGreater {

  const char *lhs_cstr;
  const char *rhs_cstr;
  int expected_res;
} TestGreater;

TestLess greater_cases[] = {
    {"0", "0", 0},    {"10", "10", 0},  {"-10", "-10", 0},
    {"1", "-1", 1},   {"-1", "1", 0},   {"1234", "12", 1},
    {"-12", "-1", 0}, {"0", "1000", 0}, {"-1000", "0", 0},
};

void tests_greater() {
  size_t num_of_tests = sizeof(greater_cases) / sizeof(greater_cases[0]);
  for (size_t i = 0; i < num_of_tests; ++i) {

    char message[256];
    sprintf(message, "Case %zu: Input lhs %s, input rhs %s", i,
            greater_cases[i].lhs_cstr, greater_cases[i].rhs_cstr);

    BigInt *lhs = bigint_create_from_cstr(greater_cases[i].lhs_cstr);

    TEST_ASSERT_NOT_NULL_MESSAGE(lhs, message);

    BigInt *rhs = bigint_create_from_cstr(greater_cases[i].rhs_cstr);

    TEST_ASSERT_NOT_NULL_MESSAGE(rhs, message);

    TEST_ASSERT_EQUAL_INT_MESSAGE(greater_cases[i].expected_res,
                                  bigint_greater(lhs, rhs), message);

    bigint_free(lhs);
    bigint_free(rhs);
  }
}

typedef struct TestIsEqual {

  const char *lhs_cstr;
  const char *rhs_cstr;
  int expected_res;
} TestIsEqual;

TestIsEqual is_equal_cases[] = {
    {"0", "0", 1},    {"10", "10", 1},  {"-10", "-10", 1},
    {"1", "-1", 0},   {"-1", "1", 0},   {"1234", "12", 0},
    {"-12", "-1", 0}, {"0", "1000", 0}, {"-1000", "0", 0},
};

void tests_is_equal() {
  size_t num_of_tests = sizeof(is_equal_cases) / sizeof(is_equal_cases[0]);
  for (size_t i = 0; i < num_of_tests; ++i) {

    char message[256];
    sprintf(message, "Case %zu: Input lhs %s, input rhs %s", i,
            is_equal_cases[i].lhs_cstr, is_equal_cases[i].rhs_cstr);

    BigInt *lhs = bigint_create_from_cstr(is_equal_cases[i].lhs_cstr);

    TEST_ASSERT_NOT_NULL_MESSAGE(lhs, message);

    BigInt *rhs = bigint_create_from_cstr(is_equal_cases[i].rhs_cstr);

    TEST_ASSERT_NOT_NULL_MESSAGE(rhs, message);

    TEST_ASSERT_EQUAL_INT_MESSAGE(is_equal_cases[i].expected_res,
                                  bigint_is_equal(lhs, rhs), message);

    bigint_free(lhs);
    bigint_free(rhs);
  }
}

typedef struct TestLessOrEqual {

  const char *lhs_cstr;
  const char *rhs_cstr;
  int expected_res;
} TestLessOrEqual;

TestLessOrEqual less_or_equal_cases[] = {
    {"0", "0", 1},    {"10", "10", 1},  {"-10", "-10", 1},
    {"1", "-1", 0},   {"-1", "1", 1},   {"1234", "12", 0},
    {"-12", "-1", 1}, {"0", "1000", 1}, {"-1000", "0", 1},
};

void tests_less_or_equal() {
  size_t num_of_tests =
      sizeof(less_or_equal_cases) / sizeof(less_or_equal_cases[0]);
  for (size_t i = 0; i < num_of_tests; ++i) {

    char message[256];
    sprintf(message, "Case %zu: Input lhs %s, input rhs %s", i,
            less_or_equal_cases[i].lhs_cstr, less_or_equal_cases[i].rhs_cstr);

    BigInt *lhs = bigint_create_from_cstr(less_or_equal_cases[i].lhs_cstr);

    TEST_ASSERT_NOT_NULL_MESSAGE(lhs, message);

    BigInt *rhs = bigint_create_from_cstr(less_or_equal_cases[i].rhs_cstr);

    TEST_ASSERT_NOT_NULL_MESSAGE(rhs, message);

    TEST_ASSERT_EQUAL_INT_MESSAGE(less_or_equal_cases[i].expected_res,
                                  bigint_less_or_equal(lhs, rhs), message);

    bigint_free(lhs);
    bigint_free(rhs);
  }
}

typedef struct TestGreaterOrEqual {

  const char *lhs_cstr;
  const char *rhs_cstr;
  int expected_res;
} TestGreaterOrEqual;

TestGreaterOrEqual greater_or_equal_cases[] = {
    {"0", "0", 1},    {"10", "10", 1},  {"-10", "-10", 1},
    {"1", "-1", 1},   {"-1", "1", 0},   {"1234", "12", 1},
    {"-12", "-1", 0}, {"0", "1000", 0}, {"-1000", "0", 0},
};

void tests_greater_or_equal() {
  size_t num_of_tests =
      sizeof(greater_or_equal_cases) / sizeof(greater_or_equal_cases[0]);
  for (size_t i = 0; i < num_of_tests; ++i) {

    char message[256];
    sprintf(message, "Case %zu: Input lhs %s, input rhs %s", i,
            greater_or_equal_cases[i].lhs_cstr,
            greater_or_equal_cases[i].rhs_cstr);

    BigInt *lhs = bigint_create_from_cstr(greater_or_equal_cases[i].lhs_cstr);

    TEST_ASSERT_NOT_NULL_MESSAGE(lhs, message);

    BigInt *rhs = bigint_create_from_cstr(greater_or_equal_cases[i].rhs_cstr);

    TEST_ASSERT_NOT_NULL_MESSAGE(rhs, message);

    TEST_ASSERT_EQUAL_INT_MESSAGE(greater_or_equal_cases[i].expected_res,
                                  bigint_greater_or_equal(lhs, rhs), message);

    bigint_free(lhs);
    bigint_free(rhs);
  }
}

typedef struct TestAdd {

  const char *bigint_num;
  const char *addend;
  const char *result;
  char res_sign;
  size_t res_size;
  size_t res_capacity;
} TestAdd;

TestAdd add_cases[] = {
    {"0", "0", "0", '+', 1, 4},
    {"1", "1", "2", '+', 1, 4},
    {"-1", "1", "0", '+', 1, 4},
    {"5", "7", "12", '+', 2, 4},
    {"5", "-7", "2", '-', 1, 4},
    {"23", "34", "57", '+', 2, 4},
    {"34", "-23", "11", '+', 2, 4},
    {"50", "50", "100", '+', 3, 4},
    {"9999", "99", "10098", '+', 5, 8},
    {"99", "-99999", "99900", '-', 5, 10},
    {"999999", "1", "1000000", '+', 7, 12},
};

void tests_add() {
  size_t num_of_tests = sizeof(add_cases) / sizeof(add_cases[0]);
  for (size_t i = 0; i < num_of_tests; ++i) {

    char message[256];
    sprintf(message, "Case %zu: Bigint num %s, addend %s", i,
            add_cases[i].bigint_num, add_cases[i].addend);

    BigInt *bigint_num = bigint_create_from_cstr(add_cases[i].bigint_num);

    TEST_ASSERT_NOT_NULL_MESSAGE(bigint_num, message);

    BigInt *addend = bigint_create_from_cstr(add_cases[i].addend);

    TEST_ASSERT_NOT_NULL_MESSAGE(addend, message);

    bigint_add(bigint_num, addend);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(add_cases[i].res_size, bigint_num->size,
                                     message);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(add_cases[i].res_capacity,
                                     bigint_num->capacity, message);
    TEST_ASSERT_EQUAL_CHAR_MESSAGE(add_cases[i].res_sign, bigint_num->sign,
                                   message);

    TEST_ASSERT_EQUAL_INT_MESSAGE(0,
                                  _check_is_correct_buffer(bigint_num->buffer,
                                                           add_cases[i].result,
                                                           bigint_num->size),
                                  message);

    bigint_free(bigint_num);
    bigint_free(addend);
  }
}

typedef struct TestSubstract {

  const char *bigint_num;
  const char *substr;
  const char *result;
  char res_sign;
  size_t res_size;
  size_t res_capacity;
} TestSubstract;

TestSubstract substract_cases[] = {
    {"0", "0", "0", '+', 1, 4},
    {"1", "1", "0", '+', 1, 4},
    {"-1", "1", "2", '-', 1, 4},
    {"5", "7", "2", '-', 1, 4},
    {"5", "-7", "12", '+', 2, 4},
    {"23", "34", "11", '-', 2, 4},
    {"34", "-23", "57", '+', 2, 4},
    {"50", "50", "0", '+', 1, 4},
    {"9999", "99", "9900", '+', 4, 8},
    {"99", "-99999", "100098", '+', 6, 12},
    {"999999", "1", "999998", '+', 6, 12},
};

void tests_substract() {
  size_t num_of_tests = sizeof(substract_cases) / sizeof(substract_cases[0]);
  for (size_t i = 0; i < num_of_tests; ++i) {

    char message[256];
    sprintf(message, "Case %zu: Bigint num %s, substr %s", i,
            substract_cases[i].bigint_num, substract_cases[i].substr);

    BigInt *bigint_num = bigint_create_from_cstr(substract_cases[i].bigint_num);

    TEST_ASSERT_NOT_NULL_MESSAGE(bigint_num, message);

    BigInt *substr = bigint_create_from_cstr(substract_cases[i].substr);

    TEST_ASSERT_NOT_NULL_MESSAGE(substr, message);

    bigint_subtract(bigint_num, substr);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(substract_cases[i].res_size, bigint_num->size,
                                     message);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(substract_cases[i].res_capacity,
                                     bigint_num->capacity, message);
    TEST_ASSERT_EQUAL_CHAR_MESSAGE(substract_cases[i].res_sign, bigint_num->sign,
                                   message);

    TEST_ASSERT_EQUAL_INT_MESSAGE(0,
                                  _check_is_correct_buffer(bigint_num->buffer,
                                                           substract_cases[i].result,
                                                           bigint_num->size),
                                  message);

    bigint_free(bigint_num);
    bigint_free(substr);
  }
}

typedef struct TestMult {

  const char *bigint_num;
  const char *multiplyer;
  const char *result;
  char res_sign;
  size_t res_size;
  size_t res_capacity;
} TestMult;

TestMult mult_cases[] = {
    {"0", "0", "0", '+', 1, 4},
    {"1", "1", "1", '+', 1, 4},
    {"-1", "1", "1", '-', 1, 4},
    {"5", "7", "35", '+', 2, 4},
    {"5", "-7", "35", '-', 2, 4},
    {"23", "34", "782", '+', 3, 8},
    {"34", "-23", "782", '-', 3, 8},
    {"50", "50", "2500", '+', 4, 10},
    {"9999", "99", "989901", '+', 6, 12},
    {"99", "-99999", "9899901", '-', 7, 14},
    {"999999", "1", "999999", '+', 6, 14},
};

void tests_mult() {
  size_t num_of_tests = sizeof(mult_cases) / sizeof(mult_cases[0]);
  for (size_t i = 0; i < num_of_tests; ++i) {

    char message[256];
    sprintf(message, "Case %zu: Bigint num %s, multiplyer %s", i,
            mult_cases[i].bigint_num, mult_cases[i].multiplyer);

    BigInt *bigint_num = bigint_create_from_cstr(mult_cases[i].bigint_num);

    TEST_ASSERT_NOT_NULL_MESSAGE(bigint_num, message);

    BigInt *multiplyer = bigint_create_from_cstr(mult_cases[i].multiplyer);

    TEST_ASSERT_NOT_NULL_MESSAGE(multiplyer, message);

    bigint_multiply(bigint_num, multiplyer);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(mult_cases[i].res_size, bigint_num->size,
                                     message);
    TEST_ASSERT_EQUAL_size_t_MESSAGE(mult_cases[i].res_capacity,
                                     bigint_num->capacity, message);
    TEST_ASSERT_EQUAL_CHAR_MESSAGE(mult_cases[i].res_sign, bigint_num->sign,
                                   message);

    TEST_ASSERT_EQUAL_INT_MESSAGE(0,
                                  _check_is_correct_buffer(bigint_num->buffer,
                                                           mult_cases[i].result,
                                                           bigint_num->size),
                                  message);

    bigint_free(bigint_num);
    bigint_free(multiplyer);
  }
}


int main() {

  UNITY_BEGIN();

  printf("\n");

  RUN_TEST(test_create);
  RUN_TEST(tests_create_from_num);
  RUN_TEST(tests_create_from_cstr);
  RUN_TEST(tests_create_copy);
  RUN_TEST(tests_less);
  RUN_TEST(tests_greater);
  RUN_TEST(tests_is_equal);
  RUN_TEST(tests_less_or_equal);
  RUN_TEST(tests_greater_or_equal);
  RUN_TEST(tests_add);
  RUN_TEST(tests_substract);
  RUN_TEST(tests_mult);

  return UNITY_END();
}
