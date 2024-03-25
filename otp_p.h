#pragma once

#include <stdio.h>

#include "otp.h"

typedef enum { UNDEFINED, KEY, ENCODE, DECODE } CommandType;

typedef struct {
    int key_length;
    char* key_file;
    FILE* kf;
} KeyArgs;

typedef struct {
    char* key_file;
    char* input_file;
    char* output_file;
    FILE* kf;
    FILE* inf;
    FILE* outf;
} EncodeDecodeArgs;

typedef union {
    KeyArgs key_args;
    EncodeDecodeArgs encode_decode_args;
} CommandArgs;

typedef struct {
    CommandType command;
    int verbosity;
    unsigned int seed;
    CommandArgs args;
} OtpCommand;

void print_help_then_exit(int status);