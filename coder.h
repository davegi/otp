#pragma once

#include "otp_p.h"

char keygen_symbol(const const_string* alphabet);
const const_string* generate_key(const const_string* key, const const_string* alphabet, keygen_symbol_func keygen_symbol, unsigned int seed);


const const_string retrieve_key(FILE* key_file);

bool encode_file(FILE* plain_text_file, FILE* cipher_text_file, const char* key, const const_string* alphabet);
bool decode_file(FILE* cipher_text_file, FILE* plain_text_file, const char* key, const const_string* alphabet);

char* encode_string(const char* plaintext, const char* key, const const_string* alphabet);
char* decode_string(const char* cipher_text, const char* key, const const_string* alphabet);

char encode_symbol(char plaintext_char, char key_char, const const_string* alphabet);
char decode_symbol(char cipher_text_char, char key_char, const const_string* alphabet);

typedef int(symbol_func)(int message_index, int key_index, size_t alphabet_length);
char code_symbol(char in, char key_char, const const_string* alphabet, symbol_func f);
int encode_symbol_func(int message_index, int key_index, size_t alphabet_length);
int decode_symbol_func(int message_index, int key_index, size_t alphabet_length);

typedef bool(file_coder_f)(FILE* in, FILE* out, const char* key, const const_string* alphabet);
bool coder(OtpCommand* command, file_coder_f f);