/*bool encode(FILE* fd_key) { 
    assert(fd_key);
    off_t key_length = get_file_length(KEYFILE);
    //insert check that key_length isn't -1
    assert(key_length != -1); //happy case!
    //bool flag = false;

    //read entire key file into an array
    char buff[key_length]; //QUESTION: before we couldn't do this? --> depends on version of c compiler using --> most modern c compilers will possibly support this 
    //int fd = fileno()
    size_t chars_read = fread(&buff, sizeof(char), key_length, fd_key);
    assert(chars_read > 0); //can use if statement  to change flag to false 

    //read streaming input from user keyboard/STDIN
    char ch = '\0';
    int key_index_counter = 0;
    while ((read(STDIN_FILENO, &ch, 1) > 0 && ch != '\n') && key_index_counter < key_length) { //FIXME  '\n' can't be used bc its a hack 
        char returned_char = encode_char(ch, buff[key_index_counter]);
        key_index_counter++;
        assert(returned_char);
    }

    return true;*/

    //focus on UI: function doesn't have to do anything; write minimal amount of code for the fx to co pile and execute
    //in this instance, return True or False but very very very simple --> treat as black box
    //only thing know about encode is it returns a bool and takes in a FILE*
    //UI and creation of stubs is one liners ONLY to help build main fx 

    //write stubs first ensuring compile properly ---> move to TDD --> implementation

    /*TDD Development (pure)
    1. write a unit test
        //encode should fail with null fd()
            //have to ref encode fx
    2. create a header file for subject under test (SUT) --> declaration for the encode fx --> COMPILE
    3. write a stub (abs bare min) that implements/defines SUT aka encode (now have .C file to go with .h file) ---> LINK bc have definition/implementation
    --UNIT TEST CAN RUN AT THIS POINT --> BUT WILL FAIL BC NO IMPLEMENTATION--
    4. add code to function until unit test under question passes
    5. create anotehr unit test, declaration, defintion, etc. --> repeat 
    --each steop until get the test to pass--

    DG Approach
    start with 2
        then go create unit tests
        focus: leverage tooling of IDE and intellisense
    
}

bool decode(FILE* key_fd) { //TODO
    assert(key_fd);
    bool flag = false;

    //secure length of ciphertext
    off_t ciphertext_len = get_file_length(CIPHERTEXT);
    //secure length of key file
    off_t key_length = get_file_length(KEYFILE);
    
    //read in ciphertext    
    char ciphertext_buff[ciphertext_len];
    FILE *fp = fopen(CIPHERTEXT, "r");
    size_t cipher_chars_read = fread(&ciphertext_buff, sizeof(char), ciphertext_len, fp);
    assert(cipher_chars_read > 0);
    
    //read in key file
    char key_buff[key_length];
    size_t key_chars_read = fread(&key_buff, sizeof(char), key_length, key_fd);
    assert(key_chars_read > 0);

    int n = 0;
    while(n < ciphertext_len && n < key_length) { //both or just key length necessary for if statement
        char value = decode_char(ciphertext_buff[n], key_buff[n]);
        assert(value > 96 && value < 123);
        printf("letters passed to decode: %c and %c\n", ciphertext_buff[n], key_buff[n]);
        n++;
    }
    
    return flag;
}*/