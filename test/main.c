#include "../src/big_int.h"
#include "unity.h"
#include "unity_internals.h"
#include <assert.h>

void setUp() {}
void tearDown() {}

void test_bigint_two_zero_sum() {
  BigInt *n1 = bigint_create_from_num(0);
  BigInt *n2 = bigint_create_from_num(0);

  BigInt *res = bigint_sum(n1, n2);

  TEST_ASSERT_EQUAL_STRING("0", res->ptr); 
  TEST_ASSERT_EQUAL_CHAR('+', res->sign);
  TEST_ASSERT_EQUAL_INT(1, res->size);

  bigint_free(n1);
  bigint_free(n2);
  bigint_free(res);
}

void test_bigint_two_zero_diff() {
  BigInt *n1 = bigint_create_from_num(0);
  BigInt *n2 = bigint_create_from_num(0);

  BigInt *res = bigint_difference(n1, n2);

  TEST_ASSERT_EQUAL_STRING("0", res->ptr); 
  TEST_ASSERT_EQUAL_CHAR('+', res->sign);
  TEST_ASSERT_EQUAL_INT(1, res->size);

  bigint_free(n1);
  bigint_free(n2);
  bigint_free(res);
}

void test_bigint_zero_plus_num() {
  BigInt *n1 = bigint_create_from_num(0);
  BigInt *n2 = bigint_create_from_num(1);

  BigInt *res = bigint_sum(n1, n2);

  TEST_ASSERT_EQUAL_STRING("1", res->ptr); 
  TEST_ASSERT_EQUAL_CHAR('+', res->sign);
  TEST_ASSERT_EQUAL_INT(1, res->size);

  bigint_free(n1);
  bigint_free(n2);
  bigint_free(res);
}

void test_bigint_num_plus_zero() {
  BigInt *n1 = bigint_create_from_num(10);
  BigInt *n2 = bigint_create_from_num(0);

  BigInt *res = bigint_sum(n1, n2);

  TEST_ASSERT_EQUAL_STRING("10", res->ptr); 
  TEST_ASSERT_EQUAL_CHAR('+', res->sign);
  TEST_ASSERT_EQUAL_INT(2, res->size);

  bigint_free(n1);
  bigint_free(n2);
  bigint_free(res);
}

void test_bigint_zero_minus_num() {
  BigInt *n1 = bigint_create_from_num(0);
  BigInt *n2 = bigint_create_from_num(10);

  BigInt *res = bigint_difference(n1, n2);

  TEST_ASSERT_EQUAL_STRING("10", res->ptr); 
  TEST_ASSERT_EQUAL_CHAR('-', res->sign);
  TEST_ASSERT_EQUAL_INT(2, res->size);

  bigint_free(n1);
  bigint_free(n2);
  bigint_free(res);
}

void test_bigint_num_minus_zero() {
  BigInt *n1 = bigint_create_from_num(34);
  BigInt *n2 = bigint_create_from_num(0);

  BigInt *res = bigint_difference(n1, n2);

  TEST_ASSERT_EQUAL_STRING("34", res->ptr); 
  TEST_ASSERT_EQUAL_CHAR('+', res->sign);
  TEST_ASSERT_EQUAL_INT(2, res->size);

  bigint_free(n1);
  bigint_free(n2);
  bigint_free(res);
}

void test_bigint_litte_num_plus() {
  BigInt *n1 = bigint_create_from_num(5);
  BigInt *n2 = bigint_create_from_num(3);

  BigInt *res = bigint_sum(n1, n2);

  TEST_ASSERT_EQUAL_STRING("8", res->ptr); 
  TEST_ASSERT_EQUAL_CHAR('+', res->sign);
  TEST_ASSERT_EQUAL_INT(1, res->size);

  bigint_free(n1);
  bigint_free(n2);
  bigint_free(res);
}

void test_bigint_litte_num_minus() {
  BigInt *n1 = bigint_create_from_num(5);
  BigInt *n2 = bigint_create_from_num(3);

  BigInt *res = bigint_difference(n1, n2);

  TEST_ASSERT_EQUAL_STRING("2", res->ptr); 
  TEST_ASSERT_EQUAL_CHAR('+', res->sign);
  TEST_ASSERT_EQUAL_INT(1, res->size);

  bigint_free(n1);
  bigint_free(n2);
  bigint_free(res);
}

void test_bigint_litte_num_negative_res() {
  BigInt *n1 = bigint_create_from_num(3);
  BigInt *n2 = bigint_create_from_num(5);

  BigInt *res = bigint_difference(n1, n2);

  TEST_ASSERT_EQUAL_STRING("2", res->ptr); 
  TEST_ASSERT_EQUAL_CHAR('-', res->sign);
  TEST_ASSERT_EQUAL_INT(1, res->size);

  bigint_free(n1);
  bigint_free(n2);
  bigint_free(res);
}

void test_bigint_negative_plus_positive() {
  BigInt *n1 = bigint_create_from_num(-10);
  BigInt *n2 = bigint_create_from_num(5);

  BigInt *res = bigint_sum(n1, n2);

  TEST_ASSERT_EQUAL_STRING("5", res->ptr); 
  TEST_ASSERT_EQUAL_CHAR('-', res->sign);
  TEST_ASSERT_EQUAL_INT(1, res->size);

  bigint_free(n1);
  bigint_free(n2);
  bigint_free(res);
}

void test_bigint_positive_plus_negative() {
  BigInt *n1 = bigint_create_from_string("15");
  BigInt *n2 = bigint_create_from_string("-10");

  BigInt *res = bigint_sum(n1, n2);

  TEST_ASSERT_EQUAL_STRING("5", res->ptr); 
  TEST_ASSERT_EQUAL_CHAR('+', res->sign);
  TEST_ASSERT_EQUAL_INT(1, res->size);

  bigint_free(n1);
  bigint_free(n2);
  bigint_free(res);
}

void test_bigint_negative_plus_negative() {
  BigInt *n1 = bigint_create_from_string("-25");
  BigInt *n2 = bigint_create_from_string("-10");

  BigInt *res = bigint_sum(n1, n2);

  TEST_ASSERT_EQUAL_STRING("35", res->ptr); 
  TEST_ASSERT_EQUAL_CHAR('-', res->sign);
  TEST_ASSERT_EQUAL_INT(2, res->size);

  bigint_free(n1);
  bigint_free(n2);
  bigint_free(res);
}

void test_bigint_negative_minus_positive() {
  BigInt *n1 = bigint_create_from_string("-105");
  BigInt *n2 = bigint_create_from_string("10");

  BigInt *res = bigint_difference(n1, n2);

  TEST_ASSERT_EQUAL_STRING("115", res->ptr); 
  TEST_ASSERT_EQUAL_CHAR('-', res->sign);
  TEST_ASSERT_EQUAL_INT(3, res->size);

  bigint_free(n1);
  bigint_free(n2);
  bigint_free(res);
}

void test_bigint_positive_minus_negative() {
  BigInt *n1 = bigint_create_from_string("10");
  BigInt *n2 = bigint_create_from_string("-10");

  BigInt *res = bigint_difference(n1, n2);

  TEST_ASSERT_EQUAL_STRING("20", res->ptr); 
  TEST_ASSERT_EQUAL_CHAR('+', res->sign);
  TEST_ASSERT_EQUAL_INT(2, res->size);

  bigint_free(n1);
  bigint_free(n2);
  bigint_free(res);
}

void test_bigint_negative_minus_negative() {
  BigInt *n1 = bigint_create_from_string("-105");
  BigInt *n2 = bigint_create_from_string("-10");

  BigInt *res = bigint_difference(n1, n2);

  TEST_ASSERT_EQUAL_STRING("95", res->ptr); 
  TEST_ASSERT_EQUAL_CHAR('-', res->sign);
  TEST_ASSERT_EQUAL_INT(2, res->size);

  bigint_free(n1);
  bigint_free(n2);
  bigint_free(res);
}

int main() {

  UNITY_BEGIN();

  RUN_TEST(test_bigint_two_zero_sum);
  RUN_TEST(test_bigint_two_zero_diff);
  RUN_TEST(test_bigint_zero_plus_num);
  RUN_TEST(test_bigint_num_plus_zero);
  RUN_TEST(test_bigint_zero_minus_num);
  RUN_TEST(test_bigint_num_minus_zero);
  RUN_TEST(test_bigint_litte_num_plus);
  RUN_TEST(test_bigint_litte_num_minus);
  RUN_TEST(test_bigint_litte_num_negative_res);
  RUN_TEST(test_bigint_negative_plus_positive);
  RUN_TEST(test_bigint_positive_plus_negative);
  RUN_TEST(test_bigint_negative_plus_negative);
  RUN_TEST(test_bigint_negative_minus_negative);
  RUN_TEST(test_bigint_positive_minus_negative);
  RUN_TEST(test_bigint_negative_minus_positive);

  return UNITY_END();
}
