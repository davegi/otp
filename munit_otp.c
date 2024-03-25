#include <stddef.h>
#include <time.h>

#include "munit/munit.h"
#include "otp.h"

#define UNUSED(x) (void)(x)
#define UNUSED_PARAMS(x, y) UNUSED(x), UNUSED(y)

#define min(a, b) (((a) < (b)) ? (a) : (b))

#define TEST(name) \
    { (char*)(#name), name, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }

#define PDF_ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define PDF_PT "HELLO"
#define PDF_KEY "XMCKL"
#define PDF_CT "EQNVZ"

const const_string pdf_pt = {.text = PDF_PT, .text_length = sizeof(PDF_PT) - 1};
const const_string pdf_key = {.text = PDF_KEY, .text_length = sizeof(PDF_KEY) - 1};
const const_string pdf_ct = {.text = PDF_CT, .text_length = sizeof(PDF_CT) - 1};
const const_string pdf_alphabet = {.text = PDF_ALPHABET, .text_length = sizeof(PDF_ALPHABET) - 1};

char keygen_symbol(const const_string* alphabet);

static MunitResult test_nil(const MunitParameter params[], void* data) {
    UNUSED_PARAMS(params, data);
    // arrange
    // act
    // assert
    return MUNIT_OK;
}

static MunitResult test_encode_symbol_pdf(const MunitParameter params[], void* data) {
    UNUSED_PARAMS(params, data);
    // arrange
    size_t symbol_count = pdf_pt.text_length;
    const char* expected = {pdf_ct.text};
    char actual[symbol_count + 1];
    // action
    for (size_t i = 0; i < symbol_count; i++) {
        actual[i] = encode_symbol(pdf_pt.text[i], pdf_key.text[i], &pdf_alphabet);
    }
    actual[symbol_count] = '\0';
    // assert
    munit_assert_string_equal(actual, expected);

    return MUNIT_OK;
}

static MunitResult test_encode_pdf(const MunitParameter params[], void* data) {
    UNUSED_PARAMS(params, data);
    // arrange
    const char* expected = {pdf_ct.text};
    // action
    char* actual = encode_string(pdf_pt.text, pdf_key.text, &pdf_alphabet);
    // assert
    munit_assert_string_equal(actual, expected);

    free(actual);

    return MUNIT_OK;
}

static MunitResult test_decode_pdf(const MunitParameter params[], void* data) {
    UNUSED_PARAMS(params, data);
    // arrange
    const char* expected = {pdf_pt.text};
    // action
    char* actual = decode_string(pdf_ct.text, pdf_key.text, &pdf_alphabet);
    // assert
    munit_assert_string_equal(actual, expected);

    free(actual);

    return MUNIT_OK;
}

static MunitResult test_decode_symbol_pdf(const MunitParameter params[], void* data) {
    UNUSED_PARAMS(params, data);
    // arrange
    size_t symbol_count = pdf_ct.text_length;
    const char* expected = {pdf_pt.text};
    char actual[symbol_count + 1];
    // action
    for (size_t i = 0; i < symbol_count; i++) {
        actual[i] = decode_symbol(pdf_ct.text[i], pdf_key.text[i], &pdf_alphabet);
    }
    actual[symbol_count] = '\0';
    // assert
    munit_assert_string_equal(expected, actual);

    return MUNIT_OK;
}

void compute_key_pdf(const char* plain, const char* cipher, char* key, const const_string* alphabet) {
    size_t cnt = min(strlen(plain), strlen(cipher));
    for (int i = 0; i < cnt; i++) {
        int plain_index = strchr(alphabet->text, plain[i]) - alphabet->text;
        int cipher_index = strchr(alphabet->text, cipher[i]) - alphabet->text;
        int shift = (cipher_index - plain_index + alphabet->text_length) % alphabet->text_length;
        key[i] = alphabet->text[shift];
    }
    key[cnt] = '\0';  // Null-terminate the key string
}

char keygen_symbol_test(const const_string* alphabet) {
    static size_t next_char = 0;
    char ch = pdf_key.text[next_char++ % pdf_key.text_length];
    return ch;
}

static MunitResult test_keygen_pdf(const MunitParameter params[], void* data) {
    // arrange
    UNUSED_PARAMS(params, data);
    // arrange
    size_t symbol_count = pdf_key.text_length;
    const char* expected = {pdf_key.text};
    char actual[symbol_count + 1];
    // act
    for (size_t i = 0; i < symbol_count; i++) {
        actual[i] = keygen_symbol_test(&pdf_alphabet);
    }
    actual[symbol_count] = '\0';
    // assert
    munit_assert_string_equal(expected, actual);

    return MUNIT_OK;
}

static MunitTest pdf_suite_tests[] = {TEST(test_encode_symbol_pdf),
                                      TEST(test_decode_symbol_pdf),
                                      TEST(test_keygen_pdf),
                                      TEST(test_encode_pdf),
                                      TEST(test_decode_pdf),
                                      TEST(test_nil),
                                      {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static const MunitSuite pdf_suite[] = {{(char*)"pdf/", pdf_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE}, NULL};

static MunitResult test_ot_pdf_example_passes(const MunitParameter params[], void* data) {
    (void)params;
    (void)data;
    // arrange
    char letter = 'D';
    char key_char = 'X';
    char plaintext_char = 'H';
    char* new_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    // action
    // char result =
    // encode_symbol(plaintext_char, key_char, new_alphabet, ALPHA_LEN);
    // assert
    // munit_assert_char(result, ==, letter);

    return MUNIT_OK;
}

static MunitResult test_different_alphabet(const MunitParameter params[], void* data) {
    (void)params;
    (void)data;
    // arrange
    char letter = 'E';
    char key_char = 'X';
    char plaintext_char = 'H';
    char* new_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    // action
    // char result = encode_symbol(plaintext_char, key_char, new_alphabet, strlen(new_alphabet));
    // assert
    // munit_assert_char(result, ==, letter);

    return MUNIT_OK;
}

static MunitResult test_different_alpha_len(const MunitParameter params[], void* data) {
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
static MunitResult test_with_numbers_instead_of_chars(const MunitParameter params[], void* data) {
    (void)params;
    (void)data;
    // arrange
    int value1 = 12;
    int value2 = 3;
    // action
    char result = encode_symbol(value1, value2, &default_alphabet);
    // assert
    munit_assert_false(result);

    return MUNIT_OK;
}

static MunitResult encode_one_char(const MunitParameter params[], void* data) {
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

static MunitResult numb_greater_than_zero(const MunitParameter params[], void* data) {
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

static MunitResult numb_less_than_alpha_length(const MunitParameter params[], void* data) {
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
static MunitResult test_get_random(const MunitParameter params[], void* data) {
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
static MunitResult is_result_positive(const MunitParameter params[], void* data) {
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
static MunitResult is_returned_char_in_alphabet(const MunitParameter params[], void* data) { return MUNIT_OK; }

static void* test_setup(const MunitParameter params[], void* user_data) { return NULL; }

// test_compare_tear_down(void* fixture) {
//   munit_assert_ptr_equal(fixture, (void*)(uintptr_t)0xdeadbeef);
// }

static MunitResult test_genkey_default(const MunitParameter params[], void* data) {
    // arrange
    const_string abc = default_alphabet;
    // act
    const const_string* key = generate_key((const const_string[1]){abc.text_length}, &abc, keygen_symbol, time(NULL));
    // assert

    return MUNIT_OK;
}

// void key_to_indices(const char* key, char* alphabet, int* indices, int length) {
//     for (int i = 0; i < length; i++) {
//         indices[i] = strchr(alphabet, key[i]) - alphabet;
//     }
// }

static MunitTest test_suite_tests[] = {  // TEST(test_with_numbers_instead_of_chars),
                                         //                                    TEST(test_different_alpha_len),
                                         //                                    TEST(test_different_alphabet),
                                         //                                    TEST(test_ot_pdf_example_passes),
                                         //                                    TEST(numb_greater_than_zero),
                                         //                                    TEST(numb_less_than_alpha_length),
                                         //                                    TEST(test_get_random),
                                         //                                    TEST(is_result_positive),
    TEST(test_genkey_default),
    TEST(test_nil),
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

// const MunitSuite* pdf[] = {&pdf_suite, NULL};

// static const MunitSuite other_suites[] = {{"/second", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE},
//                                           {NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE}};

const MunitSuite test_suite = {(char*)"/otp/", test_suite_tests, (MunitSuite*)pdf_suite, 1, MUNIT_SUITE_OPTION_NONE};
