#include "otp.h"
#include "munit/munit.h"

#include <stdio.h>
#include <sys/stat.h>

off_t get_file_length(const char* path) {
    struct stat statbuf;
    if (stat(path, &statbuf) == 0) {
        return statbuf.st_size;
    }
    fprintf(stderr, "failure to access file\n");
    
    return -1;
}

int get_char_index(char text_char, const char* alphabet, size_t alpha_length) {

    for (int i = 0; i < (int)alpha_length; i++) {
        if (text_char == alphabet[i]) {
            return i;
        }
    }
    // fprintf(stderr, "error: no value for char\n");

    return -1;
}