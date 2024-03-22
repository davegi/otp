#include <stddef.h>

#include "munit/munit.h"
#include "otp.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))

static MunitResult test_ot_pdf_example_passes(const MunitParameter params[],
                                              void *data) {
  (void)params;
  (void)data;
  // arrange
  char letter = 'D';
  char key_char = 'X';
  char plaintext_char = 'H';
  char *new_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
  // action
  // char result =
  // encode_symbol(plaintext_char, key_char, new_alphabet, ALPHA_LEN);
  // assert
  // munit_assert_char(result, ==, letter);

  return MUNIT_OK;
}

static MunitResult test_different_alphabet(const MunitParameter params[],
                                           void *data) {
  (void)params;
  (void)data;
  // arrange
  char letter = 'E';
  char key_char = 'X';
  char plaintext_char = 'H';
  char *new_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  // action
  char result = encode_symbol(plaintext_char, key_char, new_alphabet,
                              strlen(new_alphabet));
  // assert
  munit_assert_char(result, ==, letter);

  return MUNIT_OK;
}

static MunitResult test_different_alpha_len(const MunitParameter params[],
                                            void *data) {
  (void)params;
  (void)data;
  // arrange
  char letter = 'E';
  char key_char = 'X';
  char plaintext_char = 'H';
  int new_alpha_len = 20;
  // action
  // char result =
  // encode_symbol(plaintext_char, key_char, ALPHABET, new_alpha_len);
  // assert
  // munit_assert_char(result, !=, letter);

  return MUNIT_OK;
}

// test with numbers
static MunitResult test_with_numbers_instead_of_chars(
    const MunitParameter params[], void *data) {
  (void)params;
  (void)data;
  // arrange
  int value1 = 12;
  int value2 = 3;
  // action
  // char result = encode_symbol(value1, value2, ALPHABET, ALPHA_LEN);
  // assert
  // munit_assert_false(result);

  return MUNIT_OK;
}

static MunitResult encode_one_char(const MunitParameter params[], void *data) {
  (void)params;
  (void)data;
  // arrange
  char plaintext_char = 'A';
  char key_char = 'Z';
  char expected_char = 'Q';  // expected answer
  // action
  // char encoded_char =
  // encode_symbol(plaintext_char, key_char, ALPHABET, ALPHA_LEN);
  // assert
  // munit_assert_char(encoded_char, ==, expected_char);

  return MUNIT_OK;
}

static MunitResult numb_greater_than_zero(const MunitParameter params[],
                                          void *data) {
  (void)params;
  (void)data;
  // arrange
  int static_num = 0;
  // int alpha_len = strlen(ALPHABET);
  // action
  // int num_from_fx = get_random_numb(alpha_len);
  // assert
  // munit_assert_int(num_from_fx, >=, static_num);

  return MUNIT_OK;
}

static MunitResult numb_less_than_alpha_length(const MunitParameter params[],
                                               void *data) {
  (void)params;
  (void)data;
  // arrange
  int static_num = 27;  // is this the best way?????
  // int alpha_len = strlen(ALPHABET);
  // action
  // int num_from_fx = get_random_numb(alpha_len);
  // assert
  // munit_assert_int(num_from_fx, <=, static_num);

  return MUNIT_OK;
}

// don't remember what our intent was with this
static MunitResult test_get_random(const MunitParameter params[], void *data) {
  // arrange
  srand(0);
  int expected[] = {6, 24, 23, 7, 7};
  // int alpha_len = strlen(ALPHABET);

  for (int i = 0; i < (int)(sizeof(expected) / sizeof(expected[0])); i++) {
    // action
    // int r = get_random_numb(alpha_len);
    // assert
    // munit_assert_int(expected[i], ==, r);
  }

  return MUNIT_OK;
}

// expect to be positive
static MunitResult is_result_positive(const MunitParameter params[],
                                      void *data) {
  (void)params;
  (void)data;
  // arrange
  // int alpha_len = strlen(ALPHABET);
  int value = 1;
  // action
  // int result = get_random_numb(alpha_len);
  // assert
  // munit_assert_int(result, >=, 0);

  return MUNIT_OK;
}

// encode
static MunitResult is_returned_char_in_alphabet(const MunitParameter params[],
                                                void *data) {
  return MUNIT_OK;
}

static void *test_setup(const MunitParameter params[], void *user_data) {
  return NULL;
}

// test_compare_tear_down(void* fixture) {
//   munit_assert_ptr_equal(fixture, (void*)(uintptr_t)0xdeadbeef);
// }

static MunitResult test_genkey_default(const MunitParameter params[],
                                       void *data) {
  // arrange
  const_string abc = alphabet;
  // act
  const const_string *key =
      generate_key((const const_string[1]){abc.text_length}, &abc);
  // assert

  return MUNIT_OK;
}

void compute_key(const char *plain, const char *cipher, char *key,
                 const const_string *alphabet) {
  size_t cnt = min(strlen(plain), strlen(cipher));
  for (int i = 0; i < cnt; i++) {
    int plain_index = strchr(alphabet->text, plain[i]) - alphabet->text;
    int cipher_index = strchr(alphabet->text, cipher[i]) - alphabet->text;
    int shift = (cipher_index - plain_index + alphabet->text_length) %
                alphabet->text_length;
    key[i] = alphabet->text[shift];
  }
  key[cnt] = '\0';  // Null-terminate the key string
}

static MunitResult test_compute_key(const MunitParameter params[], void *data) {
  // arrange
  const char plain[] = "HELLO";
  const char cipher[] = "EQNVZ";
  const char expected_key[] = "XMCKL";
  char key[6] = {'\0'};
  // act
  compute_key(plain, cipher, key, &alphabet);
  // assert
  munit_assert_string_equal(key, expected_key);

  return MUNIT_OK;
}

void key_to_indices(const char *key, char *alphabet, int *indices, int length) {
  for (int i = 0; i < length; i++) {
    indices[i] = strchr(alphabet, key[i]) - alphabet;
  }
}

static MunitResult test_nil(const MunitParameter params[], void *data) {
  // arrange
  // act
  // assert
  return MUNIT_OK;
}

#define TEST(name) \
  { (char *)(#name), name, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }

static MunitTest test_suite_tests[] = {
    // TEST(test_with_numbers_instead_of_chars),
    // TEST(test_different_alpha_len),
    // TEST(test_different_alphabet),
    // TEST(test_ot_pdf_example_passes),
    // TEST(numb_greater_than_zero),
    // TEST(numb_less_than_alpha_length),
    // TEST(test_get_random),
    // TEST(is_result_positive),
    TEST(test_compute_key),
    TEST(test_genkey_default),
    TEST(test_nil),
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

const MunitSuite test_suite = {(char *)"/otp/", test_suite_tests, NULL, 1,
                               MUNIT_SUITE_OPTION_NONE};
