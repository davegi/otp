#pragma once

#include <ctype.h>

#include "munit/munit.h"


#define EQUAL(a, b) (strcmp((a), (b)) == 0)

#ifdef DEBUG
#define ASSUME(expr, ...)                                                                                                    \
    if (!(expr)) {                                                                                                           \
        fprintf(stderr, "Assumption (%s) failed in file %s, line %d, function %s: \n", #expr, __FILE__, __LINE__, __func__); \
        fprintf(stderr, ##__VA_ARGS__);                                                                                      \
        fprintf(stderr, "\n");                                                                                               \
        fprintf(stderr, "Choose e[x]it, any other key to continue\n");                                                       \
        if (toupper(get_char()) == 'X') {                                                                                    \
            exit(EXIT_FAILURE);                                                                                              \
        }                                                                                                                    \
    }
#define STOP ASSUME(false, "should never get here")
#else
#define ASSUME(expr, ...)
#define STOP
#endif  // DEBUG

// not on Windows
#ifndef _WIN32
// non-blocking getchar() like macro
//  - [tcgetattr(3) - Linux man page](https://linux.die.net/man/3/tcgetattr)
char get_char();
#else
#error get_char() undefined for Windows.
#endif  // _WIN32

typedef struct {
    size_t text_length;
    const char* text;
} const_string;

extern const MunitSuite test_suite;
extern const const_string default_alphabet;

typedef char (*keygen_symbol_func)(const const_string*);

const const_string* generate_key(const const_string* key, const const_string* alphabet, keygen_symbol_func keygen_symbol, unsigned int seed);
char decode_symbol(char ciphertext_char, char key_char, const const_string* alphabet);
char encode_symbol(char plaintext_char, char key_char, const const_string* alphabet);
char* encode_string(const char* plaintext, const char* key, const const_string* alphabet);
char* decode_string(const char* ciphertext, const char* key, const const_string* alphabet);
