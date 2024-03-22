#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "munit/munit.h"

typedef struct {
  size_t text_length;
  const char *text;
} const_string;

extern const MunitSuite test_suite;
extern const const_string alphabet;
// #define IS_STR_EQUAL(str1, str2) (strcmp(str1, str2) == 0)
// #define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ "
// // #define ALPHABET " ZYXWVUTSRQPONMLKJIHGFEDCBA"
// #define KEYFILE "key.txt"
// #define CIPHERTEXT "ciphertext.txt"
// #define ALPHA_LEN strlen(ALPHABET)

const const_string *generate_key(const const_string *key,
                                 const const_string *alphabet);
char decode_symbol(char key_char, char ciphertext_char, const char *alphabet,
                   size_t alpha_length);
char encode_symbol(char plaintext_char, char key_char, const char *alphabet,
                   size_t alpha_length);
char *encode(const char *key, const char *plaintext, const char *alphabet,
             size_t alpha_length);
char *decode(const char *key, const char *ciphertext, const char *alphabet,
             size_t alpha_length);
// obtain random number to pull random letter from alphabet to create key
static int get_random_numb(int alpha_len) {
  int numb =
      rand() % alpha_len; // added -1 to get letter 'A'
                          // this should not be involved with the implementation
                          // of how the result of the fx gets used
  return numb;
}
