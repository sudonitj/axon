#include "../include/utils/fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include "../include/utils/config.h"
#include "../include/utils/failures.h"

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
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *source_file = NULL;
    FILE *destination_file = NULL;
    int status = EXIT_SUCCESS;
    
    char **state;
    state = (char** )malloc(STATE_SIZE * sizeof(char*));
    if (state == NULL) {
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < STATE_SIZE; i++)
    {
        state[i] = (char*)malloc(STATE_SIZE * sizeof(char));
        if(state[i] == NULL){
            fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
            for (size_t j = 0; j < i; j++) {
                free(state[j]);
            }
            free(state);
            return EXIT_FAILURE;
        }
    }
    
    init_state(argv[1], state);

    for(int i = 0; i < STATE_SIZE; i++){
        for(int j = 0; j < STATE_SIZE; j++){
            printf("%c", state[i][j]);
        }
    }

    for (size_t i = 0; i < STATE_SIZE; i++) {
        free(state[i]);
    }
    free(state);
    return status;
}