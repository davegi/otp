#include <ctype.h>
#include <getopt.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "munit/munit.h"

#include "otp.h"

// HACK: move to build cl
#define DEBUG
#undef _WIN32

#define EQUAL(a, b) (strcmp((a), (b)) == 0)

#ifdef _WIN32
#define BP() __debugbreak()
#else
#define BP() (raise(SIGTRAP))
#endif // _WIN32

#ifdef DEBUG
#define ASSUME(expr, ...)                                                      \
  if (!(expr)) {                                                               \
    fprintf(stderr,                                                            \
            "Assumption (%s) failed in file %s, line %d, function %s: \n",     \
            #expr, __FILE__, __LINE__, __func__);                              \
    fprintf(stderr, ##__VA_ARGS__);                                            \
    fprintf(stderr, "\n");                                                     \
    char choice;                                                               \
    fprintf(stderr, "Choose an action: [E]xit, [I]gnore, [D]ebug\n");          \
    scanf(" %c", &choice);                                                     \
    if (toupper(choice) == 'E') {                                              \
      exit(EXIT_FAILURE);                                                      \
    } else if (toupper(choice) == 'D') {                                       \
      BP();                                                                    \
    }                                                                          \
  }
#else
#define ASSUME(expr, ...)
#endif // DEBUG

// examples from PDF based on this
#define DEFAULT_ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

const const_string alphabet = {.text = DEFAULT_ALPHABET,
                               .text_length = sizeof(DEFAULT_ALPHABET) - 1};

typedef enum { UNDEFINED, KEY, ENCODE, DECODE } CommandType;

typedef struct {
  int key_length;
  char *key_file;
  FILE *kf;
} KeyArgs;

typedef struct {
  char *key_file;
  char *input_file;
  char *output_file;
  FILE *kf;
  FILE *inf;
  FILE *outf;
} EncodeDecodeArgs;

typedef union {
  KeyArgs key_args;
  EncodeDecodeArgs encode_decode_args;
} CommandArgs;

typedef struct {
  CommandType command;
  int verbosity;
  CommandArgs args;
} OtpCommand;

const const_string *generate_key(const const_string *key,
                                 const const_string *alphabet) {
  ASSUME(key, "NULL key");
  ASSUME(key->text_length > 0, "invalid key length (%zu)", key->text_length);
  ASSUME(alphabet, "NULL alphabet");
  ASSUME(alphabet->text_length == strlen(alphabet->text),
         "invalid key length (%zu)", key->text_length);

  // Seed the random number generator
  srand(time(NULL));

  // Allocate memory for the key - cast around const
  *((char **)&key->text) = malloc((key->text_length + 1) * sizeof(char));
  ASSUME(key->text != NULL, "Failed to allocate memory for key")
  if (key->text == NULL) {
    return NULL; // Failed to allocate memory
  }
  // Generate the key
  for (int i = 0; i < key->text_length; i++) {
    int random_index = rand() % alphabet->text_length;
    // NOTES: cast away const❗
    *(char *)&(key->text[i]) = alphabet->text[random_index];
  }

  // Null-terminate the key
  char *ch = (char *)&(key->text[key->text_length]);
  *ch = '\0';

  return key;
}

// Function to encode a character
char encode_symbol(char plain, char key, const char *alphabet, size_t length) {
  ASSUME(alphabet, "NULL alphabet");
  ASSUME(length > 0, "invalid length")

  int message_index = strchr(alphabet, plain) - alphabet;
  int key_index = strchr(alphabet, key) - alphabet;

  int encoded_index = (message_index + key_index) % length;

  return alphabet[encoded_index];
}

// Function to decode a character
char decode_symbol(char cipher, char key, const char *alphabet, size_t length) {
  ASSUME(alphabet, "NULL alphabet");
  ASSUME(length > 0, "invalid length")

  int ciphertext_index = strchr(alphabet, cipher) - alphabet;
  int key_index = strchr(alphabet, key) - alphabet;

  int decoded_index = (ciphertext_index - key_index + length) % length;

  return alphabet[decoded_index];
}

char *encode(const char *key, const char *plaintext, const char *alphabet,
             size_t alpha_length) {
  return NULL;
}

char *decode(const char *key, const char *ciphertext, const char *alphabet,
             size_t alpha_length) {
  return NULL;
}

void print_usage() {
  printf("Usage: otp <command> [options]\n");
  // ...
}

void print_help() {
  printf("Help: otp <command> [options]\n");
  // ...
}

void exit_with_message(char *message, int exit_code) {
  fprintf(stderr, "%s\n", message);
  exit(exit_code);
}
void otp(OtpCommand *command) {
  ASSUME(command, "NULL command");

  const int min_key_length = 0;
  switch (command->command) {
  case KEY: {
    KeyArgs *key_args = &command->args.key_args;
    if (key_args->key_length <= min_key_length) {
      print_help();
    } else {
      key_args->kf = (key_args->key_file == NULL)
                         ? stdout
                         : fopen(command->args.key_args.key_file, "w");
    }
    const const_string *key =
        generate_key((const_string[1]){key_args->key_length}, &alphabet);
    if (key_args->kf) {
      fclose(key_args->kf);
    }
    break;
  }
  case ENCODE:
  case DECODE:
    if (command->args.encode_decode_args.key_file == NULL) {
      print_help();
    } else {
      command->args.encode_decode_args.kf =
          (command->args.encode_decode_args.key_file == NULL)
              ? stdin
              : fopen(command->args.encode_decode_args.key_file, "r");
    }
    command->args.encode_decode_args.inf =
        (command->args.encode_decode_args.input_file == NULL)
            ? stdin
            : fopen(command->args.encode_decode_args.input_file, "r");
    command->args.encode_decode_args.outf =
        (command->args.encode_decode_args.output_file == NULL)
            ? stdout
            : fopen(command->args.encode_decode_args.output_file, "w");
    ASSUME(command->args.encode_decode_args.kf, "invlaid key file stream");
    ASSUME(command->args.encode_decode_args.inf, "invalid input file")
    ASSUME(command->args.encode_decode_args.outf, "invalid output file")
    break;
  default:
    exit_with_message("Invalid command", EXIT_FAILURE);
  }
}

OtpCommand *parse_args(OtpCommand *command, int argc, char **argv) {
  if (EQUAL(argv[1], "key")) {
    command->command = KEY;
  } else if (EQUAL(argv[1], "encode")) {
    command->command = ENCODE;
  } else if (EQUAL(argv[1], "decode")) {
    command->command = DECODE;
  } else {
    ASSUME((EQUAL(argv[1], "key")) || (EQUAL(argv[1], "encode")) ||
               (EQUAL(argv[1], "decode")),
           "Invalid command");
    exit_with_message("Invalid command", EXIT_FAILURE);
  }

  optind++;

  int c;
  while ((c = getopt_long(argc, argv, "k:f:p:c:v?", NULL, NULL)) != -1) {
    switch (c) {
    case 'k':
      ASSUME(command->command == KEY && command->args.key_args.key_length == 0,
             "Invalid or duplicate 'key-length' argument");
      if (command->command != KEY || command->args.key_args.key_length != 0) {
        exit_with_message("Invalid or duplicate 'key-length' argument", 1);
      }
      command->args.key_args.key_length = atoi(optarg);
      break;

    case 'f':
      ASSUME(command->args.key_args.key_file == NULL &&
                 command->args.encode_decode_args.key_file == NULL,
             "Duplicate 'key-file' argument");
      if (command->args.key_args.key_file != NULL ||
          command->args.encode_decode_args.key_file != NULL) {
        exit_with_message("Duplicate 'key-file' argument", 1);
      }
      if (command->command == KEY) {
        command->args.key_args.key_file = optarg;
      } else {
        command->args.encode_decode_args.key_file = optarg;
      }
      break;

    case 'p':
      ASSUME((command->command == ENCODE || command->command == DECODE) &&
                 command->args.encode_decode_args.input_file == NULL,
             "Invalid or duplicate 'plain-text-file' argument");
      if (command->command == KEY ||
          command->args.encode_decode_args.input_file != NULL) {
        exit_with_message("Invalid or duplicate 'plain-text-file' argument", 1);
      }
      command->args.encode_decode_args.input_file = optarg;
      break;

    case 'c':
      ASSUME((command->command == ENCODE || command->command == DECODE) &&
                 command->args.encode_decode_args.output_file == NULL,
             "Invalid or duplicate 'cipher-text-file' argument");
      if (command->command == KEY ||
          command->args.encode_decode_args.output_file != NULL) {
        exit_with_message("Invalid or duplicate 'cipher-text-file' argument",
                          1);
      }
      command->args.encode_decode_args.output_file = optarg;
      break;

    case 'v':
      command->verbosity++;
      break;

    case '?':
      print_usage();
      return 0;

    default:
      print_help();
      return 0;
    }
  }
  return command;
}

int main(int argc, char **argv) {

  if (argc < 2) {
    print_usage();
    return 1;
  }

  if (EQUAL(argv[1], "test")) {
    return munit_suite_main(&test_suite, (void *)"µnit", argc - 1, argv + 1);
  }

  OtpCommand *command = parse_args((OtpCommand[1]){}, argc, argv);

  otp(command);

  return 0;
}