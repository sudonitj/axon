#include "../../include/utils/memory.h"
#include "../../include/utils/failures.h"
#include <stdio.h>

char** allocate_state_memory(size_t rows, size_t cols){
    char** state;
    state = (char**)malloc(rows * sizeof(char*));
    if(state == NULL){
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        return NULL;
    }
    for(size_t i = 0; i < rows; i++){
        state[i] = (char*)malloc(cols*sizeof(char));
        if(state[i] == NULL){
            fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
            for(size_t j = 0; j < i; j++){
                free(state[j]);
            }
            free(state);
            return NULL;
        }
    }
    return state;
}

void free_state_memory(char** state, size_t rows){
    if (state == NULL) return;
    
    for (size_t i = 0; i < rows; i++) {
        free(state[i]);
    }
    free(state);
}
