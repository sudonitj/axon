#include "../include/common/failures.h"
#include "../include/crypto/password.h"
#include "../include/crypto/password_simd.h"
#include "../include/common/config.h"
#include "../include/common/optimization.h"
#include <string.h>
#include <stdlib.h>

void chunker_original(char* key, int size, char* xor_res){
    if (key == NULL || xor_res == NULL || size <= 0) return;
    if (*key == '\0') return;

    char* key_local = malloc(size + 1);
    if (key_local == NULL) return;
    memset(key_local, 0, size + 1);
    strncpy(key_local, key, size);

    int key_len = strlen(key);
    
    for (int i = 0; i < size && key_local[i] != '\0'; i++) {
        xor_res[i] ^= key_local[i];
    }

    free(key_local);
    if (strlen(key) > size) {
        chunker_original(key + size, size, xor_res);
    }
}

typedef void (*chunker_func_t)(char*, int, char*);

static chunker_func_t optimal_chunker = NULL;

void chunker(char* key, int size, char* xor_res) {
    if (optimal_chunker == NULL) {
        optimal_chunker = get_optimal_implementation(
            (void*)chunker_original, 
            (void*)chunker_sse2, 
            (void*)chunker_avx, 
            (void*)chunker_avx2,
            &g_opt_settings);
    }
    
    optimal_chunker(key, size, xor_res);
}




char* validate_password(const char* password){
    if(password == NULL) {
        fprintf(stderr, NULL_PASSWORD);
        return NULL;
    }

    size_t password_len = strlen(password);
    size_t target_len = STATE_SIZE * STATE_SIZE;

    if(password_len > target_len){
        char* xor_res = calloc(target_len + 1, sizeof(char));
        if (!xor_res) {
            fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
            return NULL;
        }
        char* password_copy = strdup(password);
        if (!password_copy) {
            fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
            free(xor_res);
            return NULL;
        }
        chunker(password_copy, target_len, xor_res);
        free(password_copy);
        xor_res[target_len] = '\0';
        
        return xor_res;
    }

    if(password_len < target_len){
        char* updated_password = malloc(target_len + 1); 
        if (!updated_password) {
            fprintf(stderr,MEMORY_ALLOCATION_FAILURE);
            return NULL;
        }
        strcpy(updated_password, password);
        size_t position = password_len;
        while(position < target_len) {
            for(size_t i = 0; i < password_len && position < target_len; i++, position++){
                updated_password[position] = password[i];
            }
        }
        updated_password[target_len] = '\0';
        return updated_password; 
    }

    char* password_copy = strdup(password);
    if (!password_copy) {
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        return NULL;
    }
    return password_copy;
}
