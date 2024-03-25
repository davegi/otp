
#include "otp.h"

#include <getopt.h>
#include <stdbool.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "coder.h"
#include "otp_p.h"

// not on Windows
#ifndef _WIN32
// non-blocking getchar() - i.e., returns the first key pressed
//  - [tcgetattr(3) - Linux man page](https://linux.die.net/man/3/tcgetattr)
char get_char() {
    // get and save the terminal settings
    struct termios old_tio, new_tio;
    int rc = tcgetattr(STDIN_FILENO, &old_tio);
    ASSUME(rc != -1, "tcgetattr error");
    new_tio = old_tio;
    // update the settings to non-blocking (non-canonical mode) and no echo
    new_tio.c_lflag &= (~ICANON & ~ECHO);
    rc = tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    ASSUME(rc != -1, "tcsetattr error");

    // get the char
    int c = getchar();

    // restore the original terminal settings
    rc = tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    ASSUME(rc != -1, "tcsetattr error");

    return (char)c;
}
#else
#error get_char() undefined for Windows.
#endif  // _WIN32

void print_usage() { printf("Usage: otp <command> [options]\n"); }

void print_help_then_exit(int status) {
    printf("Help: otp <command> [options]\n");
    exit(status);
}

void exit_with_message(char* message, int exit_code) {
    fprintf(stderr, "%s\n", message);
    exit(exit_code);
    STOP;
}

bool gen_key(const OtpCommand* command) {
    KeyArgs* key_args = (KeyArgs*)&(command->args.key_args);
    const int min_key_length = 0;
    if (key_args->key_length <= min_key_length) {
        print_help_then_exit(EXIT_FAILURE);
    } else {
        key_args->kf = (key_args->key_file == NULL) ? stdout : fopen(command->args.key_args.key_file, "w");
    }
    const const_string* key = generate_key((const_string[1]){key_args->key_length}, &default_alphabet, keygen_symbol, command->seed == 0 ? time(NULL) : command->seed);
    fprintf(key_args->kf, "%s\n", key->text);
    if (key_args->kf != stdout) {
        fclose(key_args->kf);
    }
    return true;
}

void otp(OtpCommand* command) {
    ASSUME(command, "NULL command");

    switch (command->command) {
    case KEY: {
        bool success = gen_key(command);
        ASSUME(success, "gen_key() failed");
        break;
    }
    case ENCODE: {
        bool success = coder(command, encode_file);
        ASSUME(success, "encode_file() failed");
        break;
    }
    case DECODE: {
        bool success = coder(command, decode_file);
        ASSUME(success, "decode_file() failed");
        break;
    }
    default:
        exit_with_message("Invalid command", EXIT_FAILURE);
    }
}

CommandType parse_command_type(const char* cmd) {
    ASSUME((EQUAL(cmd, "key")) || (EQUAL(cmd, "encode")) || (EQUAL(cmd, "decode")), "Invalid command");
    if (EQUAL(cmd, "key")) {
        return KEY;
    } else if (EQUAL(cmd, "encode")) {
        return ENCODE;
    } else if (EQUAL(cmd, "decode")) {
        return DECODE;
    } else {
        return UNDEFINED;
    }
}

OtpCommand* parse_args(OtpCommand* command, int argc, char** argv) {
    command->command = parse_command_type(argv[1]);
    if (command->command == UNDEFINED) {
        exit_with_message("Invalid command", EXIT_FAILURE);
    }
    // skip over the command before calling getopt_long()
    optind++;

    int c;
    while ((c = getopt_long(argc, argv, "k:l:p:c:s:v?", NULL, NULL)) != -1) {
        switch (c) {
        // key file
        case 'k':
            ASSUME(command->args.key_args.key_file == NULL, "duplicate key-file");
            if (command->args.key_args.key_file != NULL) {
                exit_with_message("Duplicate 'key-file' argument", 1);
            }
            if (command->command == KEY) {
                command->args.key_args.key_file = optarg;
            } else {
                command->args.encode_decode_args.key_file = optarg;
            }
            break;
        // key length
        case 'l':
            ASSUME(command->command == KEY && command->args.key_args.key_length == 0, "Invalid or duplicate 'key-length' argument");
            if (command->command != KEY || command->args.key_args.key_length != 0) {
                exit_with_message("Invalid or duplicate 'key-length' argument", 1);
            }
            command->args.key_args.key_length = atoi(optarg);
            break;
        // plaintext file
        case 'p':
            ASSUME((command->command == ENCODE && command->args.encode_decode_args.input_file == NULL) ||
                       (command->command == DECODE && command->args.encode_decode_args.output_file == NULL),
                   "Invalid or duplicate 'plain-text-file' argument");
            if (command->command == ENCODE && command->args.encode_decode_args.input_file == NULL) {
                command->args.encode_decode_args.input_file = optarg;
            } else if (command->command == DECODE || command->args.encode_decode_args.output_file == NULL) {
                command->args.encode_decode_args.output_file = optarg;
            } else {
                exit_with_message("Invalid or duplicate 'plain-text file' argument", 1);
            }
            break;
        // cipher-text file
        case 'c':
            ASSUME((command->command == ENCODE && command->args.encode_decode_args.output_file == NULL) ||
                       (command->command == DECODE && command->args.encode_decode_args.input_file == NULL),
                   "Invalid or duplicate 'plain-text-file' argument");
            if (command->command == ENCODE && command->args.encode_decode_args.output_file == NULL) {
                command->args.encode_decode_args.output_file = optarg;
            } else if (command->command == DECODE || command->args.encode_decode_args.input_file == NULL) {
                command->args.encode_decode_args.input_file = optarg;
            } else {
                exit_with_message("Invalid or duplicate 'cipher-text file' argument", 1);
            }
            break;
        // seed
        case 's':
            command->seed = atoi(optarg);
            ASSUME(command->seed != 0, "zero seed");
            break;
        // verbosity level (count)
        case 'v':
            command->verbosity++;
            break;
        // help
        case '?':
            print_help_then_exit(EXIT_FAILURE);
            STOP;
        // others get a usage message
        default:
            print_usage();
            return 0;
        }
    }
    return command;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        print_usage();
        return EXIT_SUCCESS;
    }

    if (EQUAL(argv[1], "test")) {
        return munit_suite_main(&test_suite, (void*)"µnit", argc - 1, argv + 1);
    }

    OtpCommand* command = parse_args((OtpCommand[1]){}, argc, argv);

    otp(command);

    return EXIT_SUCCESS;
}