#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// otp -e key.key

// -k (keygen) keyLength 
// -e (encode) key.key < plaintext.txt > cipher.txt
// -d (decode) key.key < ciphertext.txt > new_plaintext.txt

// diff plaintext.txt new_plaintxt.txt 

int main(int argc, char* argv[]) {
    int opt;

    while ((opt = getopt(argc, argv, "k:e:d:")) != -1) {
        switch (opt) {
            case 'k':
                printf("yup, got k\n");
                
                break;
            case 'e':
                printf("yup, got e\n");
                break;
            case 'd':
                printf("yup, got d\n");
                break;
            default:
                fprintf(stderr, "Usage: %s\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}