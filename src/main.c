#include "../include/utils/fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utils/config.h"
#include "../include/utils/failures.h"
#include "../include/utils/memory.h"
#include "../include/utils/password.h"

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
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <key>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *source_file = NULL;
    FILE *destination_file = NULL;
    int status = EXIT_SUCCESS;
    
    char **state = allocate_state_memory(STATE_SIZE, STATE_SIZE);
    
    init_state(argv[1], state);

    // for(int i = 0; i < STATE_SIZE; i++){
    //     for(int j = 0; j < STATE_SIZE; j++){
    //         printf("%c", state[i][j]);
    //     }
    // }
    char* final_pass = validate_password(argv[2]);

    printf("\nFinal password length: %zu\n", strlen(final_pass));
    for (size_t i = 0; i < strlen(final_pass); i++) {
        printf("char[%zu]: '%c' (ASCII: %d)\n", i, final_pass[i], (int)final_pass[i]);
    }
    free_state_memory(state, STATE_SIZE);
    return status;
}