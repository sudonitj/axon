#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/common/failures.h"
#include "../../include/crypto/encryptor.h"
#include "../../include/utils/conversion.h"

char* encryptor(char** state, char* final_pass, int block_size){
    for (size_t i = 0; i < block_size; i++){
        for (size_t j = 0; j < block_size; j++){
            state[i][j] ^= final_pass[i * block_size + j];
        }
    }

    char* flat_state = (char*)malloc(block_size * block_size * sizeof(char));
    if (flat_state == NULL) {
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        return NULL;
    }
    for (size_t i = 0; i < block_size; i++){
        for (size_t j = 0; j < block_size; j++){
            flat_state[i*block_size + j] = state[i][j];
        }
    }

    char* hexargs = bytes_to_hex((unsigned char*)flat_state, block_size * block_size);
    free(flat_state);
    return hexargs;
}