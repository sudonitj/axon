#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/common/failures.h"
#include "../../include/crypto/decryptor.h"
#include "../../include/utils/conversion.h"
#include "../../include/utils/memory.h"

char* decryptor(char* hex_bytes, char* final_pass, int block_size){
    size_t binary_len;
    unsigned char* binary_data = hex_to_bytes(hex_bytes, &binary_len);
    if (binary_data == NULL) {
        fprintf(stderr, "Error converting hex to bytes\n");
        return NULL;
    }
    char** state = allocate_matrix_memory(block_size, block_size);
    if (state == NULL) {
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        free(binary_data);
        return NULL;
    }
    for (size_t i = 0; i < block_size; i++) {
        for (size_t j = 0; j < block_size; j++) {
            state[i][j] = binary_data[i * block_size + j];
        }
    }
    for (size_t i = 0; i < block_size; i++) {
        for (size_t j = 0; j < block_size; j++) {
            state[i][j] ^= final_pass[i * block_size + j];
        }
    }
    free(binary_data);

    char* flat_state = (char*)malloc(block_size * block_size * sizeof(char));
    if (flat_state == NULL) {
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        free_matrix_memory(state, block_size);
        return NULL;
    }
    for (size_t i = 0; i < block_size; i++) {
        for (size_t j = 0; j < block_size; j++) {
            flat_state[i * block_size + j] = state[i][j];
        }
    }
    free_matrix_memory(state, block_size);
    return flat_state;
}