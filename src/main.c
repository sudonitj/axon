#include "../include/utils/fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/common/config.h"
#include "../include/common/failures.h"
#include "../include/utils/memory.h"
#include "../include/crypto/password.h"
#include "../include/utils/conversion.h"
#include "../include/crypto/encryptor.h"
#include "../include/crypto/decryptor.h"

#define STATE_SIZE 4

// stderr is a standard error stream in C, 
// which is used to output error messages. 
// Unlike stdout, which is buffered, stderr 
// is typically unbuffered, meaning that error
// messages are displayed immediately without 
// waiting for the buffer to fill up. 
// This ensures that error messages are printed 
// promptly, even if the program crashes 
// or terminates unexpectedly



int main(int argc, const char* argv[]) {
    // if (argc != 5) {
    //     fprintf(stderr, "Usage: %s <source_file> <destination_file> <key> <e/d>\n", argv[0]);
    //     return EXIT_FAILURE;
    // }

    FILE *source_file = NULL;
    FILE *destination_file = NULL;
    int status = EXIT_SUCCESS;
    
    // char **state = allocate_matrix_memory(STATE_SIZE, STATE_SIZE);
    // init_state(argv[1], state);

    // char* final_pass = validate_password(argv[3]);

    // printf("\nFinal password length: %zu\n", strlen(final_pass));

    // char **password = allocate_matrix_memory(STATE_SIZE, STATE_SIZE);

    // for (size_t i = 0; i < STATE_SIZE; i++){
    //     for (size_t j = 0; j < STATE_SIZE; j++){
    //        state[i][j] ^= final_pass[i * STATE_SIZE + j];
    //     }
    // }
    // char* flat_state = malloc(STATE_SIZE * STATE_SIZE * sizeof(char));
    // if (flat_state == NULL) {
    //     fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
    //     free_matrix_memory(password, STATE_SIZE);
    //     free_matrix_memory(state, STATE_SIZE);
    //     return EXIT_FAILURE;
    // }

    // for (size_t i = 0; i < STATE_SIZE; i++) {
    //     for (size_t j = 0; j < STATE_SIZE; j++) {
    //         flat_state[i * STATE_SIZE + j] = state[i][j];
    //     }
    // }

    // char* hexarg = bytes_to_hex((unsigned char*)flat_state, STATE_SIZE * STATE_SIZE);
    // printf("Hex: %s\n", hexarg);

    // write_file(argv[2], hexarg, STATE_SIZE * STATE_SIZE * 2);

    // free_matrix_memory(password, STATE_SIZE);
    // free_matrix_memory(state, STATE_SIZE);

    char*** states = file_chunker(argv[1]);
    

    // if(strcmp(argv[4], "e") == 0){
    //     // perform encryption of argv[1] with argv[3] and write to argv[2]
    //     char** state = allocate_matrix_memory(STATE_SIZE, STATE_SIZE);
    //     init_state(argv[1], state);
    //     char* final_pass = validate_password(argv[3]);
    //     char* encrypted_content = encryptor(state, final_pass, STATE_SIZE);
    //     write_file(argv[2], encrypted_content, STATE_SIZE * STATE_SIZE * 2);
    // }
    // else if(strcmp(argv[4], "d") == 0){
    //     // perform decryption of argv[1] with argv[3] and write to argv[2]
    //     char* encrypted_content = read_file(argv[1]);
    //     char* final_pass = validate_password(argv[3]);
    //     char* decrypted_flat_state = decryptor(encrypted_content, final_pass, STATE_SIZE);
    //     write_file(argv[2], decrypted_flat_state, STATE_SIZE * STATE_SIZE);
    // }
    // else{
    //     printf("%s\n\n\n", argv[4]);
    //     fprintf(stderr, "Invalid operation\n");
    //     status = EXIT_FAILURE;
    // }
    return status;
}