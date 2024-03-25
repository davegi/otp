#include "otp_p.h"

#include <alloca.h>
#include <ctype.h>
#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

// examples from PDF based on this
#define DEFAULT_ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

const const_string default_alphabet = {.text = DEFAULT_ALPHABET, .text_length = sizeof(DEFAULT_ALPHABET) - 1};

#define CHECK_ALPHABET(alphabet)                   \
    ASSUME(alphabet, "NULL alphabet");             \
    ASSUME(alphabet->text, "NULL alphabet->text"); \
    ASSUME(alphabet->text_length > 0, "invalid length (%zu)", alphabet->text_length);

char keygen_symbol(const const_string* alphabet) {
    CHECK_ALPHABET(alphabet);
    int random_index = rand() % alphabet->text_length;
    return alphabet->text[random_index];
}

const const_string* generate_key(const const_string* key, const const_string* alphabet, keygen_symbol_func keygen_symbol, unsigned int seed) {
    ASSUME(key, "NULL key");
    ASSUME(key->text_length > 0, "invalid key length (%zu)", key->text_length);
    CHECK_ALPHABET(alphabet);

    // Seed the random number generator
    srand(seed);

    // Allocate memory for the key - cast around const
    *((char**)&key->text) = malloc((key->text_length + 1) * sizeof(char));
    ASSUME(key->text != NULL, "Failed to allocate memory for key")
    if (key->text == NULL) {
        return NULL;  // Failed to allocate memory
    }
    // Generate the key
    for (int i = 0; i < key->text_length; i++) {
        // NOTES: cast away const❗
        *(char*)&(key->text[i]) = keygen_symbol(alphabet);
    }

    // Null-terminate the key
    char* ch = (char*)&(key->text[key->text_length]);
    *ch = '\0';

    return key;
}

typedef int(symbol_func)(int message_index, int key_index, size_t alphabet_length);

int encode_symbol_func(int message_index, int key_index, size_t alphabet_length) { return (message_index + key_index) % alphabet_length; }

int decode_symbol_func(int message_index, int key_index, size_t alphabet_length) { return (message_index - key_index + alphabet_length) % alphabet_length; }

// Function to encode a character
char code_symbol(char in, char key_char, const const_string* alphabet, symbol_func f) {
    CHECK_ALPHABET(alphabet);

    int message_index = strchr(alphabet->text, in) - alphabet->text;
    int key_index = strchr(alphabet->text, key_char) - alphabet->text;

    int symbol_index = f(message_index, key_index, alphabet->text_length);

    return alphabet->text[symbol_index];
}

// Function to encode a character
char encode_symbol(char plaintext_char, char key_char, const const_string* alphabet) {
    CHECK_ALPHABET(alphabet);
    return code_symbol(plaintext_char, key_char, alphabet, encode_symbol_func);
}

// Function to decode a character
char decode_symbol(char cipher_text_char, char key_char, const const_string* alphabet) {
    CHECK_ALPHABET(alphabet);
    return code_symbol(cipher_text_char, key_char, alphabet, decode_symbol_func);
}

char* encode_string(const char* plaintext, const char* key, const const_string* alphabet) {
    ASSUME(plaintext, "NULL plaintext");
    ASSUME(key, "NULL key");
    CHECK_ALPHABET(alphabet);

    char* cipher_text = malloc(strlen(plaintext) + 1);
    ASSUME(cipher_text, "Failed to allocate memory for cipher-text")
    if (cipher_text == NULL) {
        return NULL;  // Failed to allocate memory
    }
    for (size_t i = 0; i < strlen(plaintext); i++) {
        cipher_text[i] = encode_symbol(plaintext[i], key[i], alphabet);
    }

    return cipher_text;
}

const const_string retrieve_key(FILE* key_file) {
    ASSUME(key_file, "NULL  key_file");
    char* key = NULL;
    size_t key_length = 0;
    ssize_t n_read = 0;
    // read and encode each line from the file
    n_read = getline(&key, &key_length, key_file);
    ASSUME(n_read > 0 && key != NULL, "i/o failure")

    return (const_string){.text = key, .text_length = n_read};
}

bool encode_file(FILE* plain_text_file, FILE* cipher_text_file, const char* key, const const_string* alphabet) {
    ASSUME(plain_text_file, "NULL plain_text_file");
    ASSUME(cipher_text_file, "NULL cipher_text_file");
    ASSUME(key, "NULL key");
    CHECK_ALPHABET(alphabet);
    char* plain_text = NULL;
    size_t plain_text_length = 0;
    ssize_t n_read = 0;
    // read and encode each line from the file
    while ((n_read = getline(&plain_text, &plain_text_length, plain_text_file)) > 0) {
        char* encoded_string = encode_string(plain_text, key, alphabet);
        ASSUME(strlen(plain_text) == strlen(encoded_string), "invalid lengths");
        size_t n_written = fwrite(encoded_string, sizeof(char), n_read, cipher_text_file);
        ASSUME(n_written == n_read, "i/o error");
        int rc = fflush(cipher_text_file);
        ASSUME(rc != -1, "fflush() failed");
        free(encoded_string);  // HACK: replace allocations in f()-s w/compound
                               // literal
    }
    free(plain_text);

    return true;
}

bool decode_file(FILE* cipher_text_file, FILE* plain_text_file, const char* key, const const_string* alphabet) {
    ASSUME(cipher_text_file, "NULL cipher_text_file");
    ASSUME(plain_text_file, "NULL plain_text_file");
    ASSUME(key, "NULL key");
    CHECK_ALPHABET(alphabet);
    char* cipher_text = NULL;
    size_t cipher_text_length = 0;
    ssize_t n_read = 0;
    // read and decode each line from the file
    while ((n_read = getline(&cipher_text, &cipher_text_length, cipher_text_file)) > 0) {
        char* decoded_string = decode_string(cipher_text, key, alphabet);
        ASSUME(strlen(cipher_text) == strlen(decoded_string), "invalid lengths");
        size_t n_written = fwrite(decoded_string, sizeof(char), n_read, plain_text_file);
        ASSUME(n_written == n_read, "i/o error");
        free(decoded_string);
    }
    free(cipher_text);

    return true;
}

char* decode_string(const char* cipher_text, const char* key, const const_string* alphabet) {
    ASSUME(cipher_text, "NULL cipher-text");
    ASSUME(key, "NULL key");
    CHECK_ALPHABET(alphabet);

    char* plaintext = malloc(strlen(cipher_text) + 1);
    ASSUME(plaintext, "Failed to allocate memory for plaintext")
    if (plaintext == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < strlen(cipher_text); i++) {
        plaintext[i] = decode_symbol(cipher_text[i], key[i], alphabet);
    }

    return plaintext;
}

typedef bool(file_coder_f)(FILE* in, FILE* out, const char* key, const const_string* alphabet);

bool coder(OtpCommand* command, file_coder_f f) {
    EncodeDecodeArgs* args = &(command->args.encode_decode_args);
    if (args->key_file == NULL) {
        print_help_then_exit(EXIT_FAILURE);
    } else {
        args->kf = (args->key_file == NULL) ? stdin : fopen(args->key_file, "r");
    }
    ASSUME(args->kf, "invalid key file stream");
    args->inf = (args->input_file == NULL) ? stdin : fopen(args->input_file, "r");
    args->outf = (args->output_file == NULL) ? stdout : fopen(args->output_file, "w");
    ASSUME(args->inf, "invalid input file stream");
    ASSUME(args->outf, "invalid output file stream");

    const const_string key = retrieve_key(args->kf);
    ASSUME(key.text, "NULL key.text");
    ASSUME(strlen(key.text) == key.text_length, "invalid key.text_length");

    bool success = f(args->inf, args->outf, key.text, &default_alphabet);
    ASSUME(success, "encode_file() failed");

    return success;
}
