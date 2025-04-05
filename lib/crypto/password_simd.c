#include "../include/crypto/password_simd.h"
#include "../include/crypto/simd_compat.h"
#include <stdlib.h>
#include <string.h>


#if defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
    #include <immintrin.h>
    void chunker_sse2(char* key, int size, char* xor_res){
        if (key == NULL || xor_res == NULL || size <= 0) return;
        if (*key == '\0') return;

        int original_key_len = strlen(key);

        char* key_local = malloc(size + 1);
        if (key_local == NULL) return;
        memset(key_local, 0, size + 1);
        strncpy(key_local, key, size);

        int key_len = strlen(key);
        
        int i = 0;
        while (i <= key_len - 16) {
            __m128i key_chunk = _mm_loadu_si128((__m128i*)&key_local[i]);
            __m128i xor_chunk = _mm_loadu_si128((__m128i*)&xor_res[i]);
            
            __m128i result = _mm_xor_si128(key_chunk, xor_chunk);
            _mm_storeu_si128((__m128i*)&xor_res[i], result);
            i += 16;
        }

        for (; i < key_len; i++) {
            xor_res[i] ^= key_local[i];
        }

        free(key_local);
        if (strlen(key) > size) {
            chunker_sse2(key + size, size, xor_res);
        }
    }

#else
    void chunker_sse2(char* key, int size, char* xor_res) {}
#endif


#if defined(__AVX__)
    #include <immintrin.h>
    void chunker_avx(char* key, int size, char* xor_res){
        if (key == NULL || xor_res == NULL || size <= 0) return;
        if (*key == '\0') return;

        int original_key_len = strlen(key);

        char* key_local = malloc(size + 1);
        if (key_local == NULL) return;
        memset(key_local, 0, size + 1);
        strncpy(key_local, key, size);

        int key_len = strlen(key);
        
        int i = 0;
        while (i <= key_len - 32) {
            __m256i key_chunk = _mm256_loadu_si256((__m256i*)&key_local[i]);
            __m256i xor_chunk = _mm256_loadu_si256((__m256i*)&xor_res[i]);
            
            __m256i result = _mm256_xor_si256(key_chunk, xor_chunk);
            _mm256_storeu_si256((__m256i*)&xor_res[i], result);
            i += 32;
        }

        while (i <= key_len - 16) {
            __m128i key_chunk = _mm_loadu_si128((__m128i*)&key_local[i]);
            __m128i xor_chunk = _mm_loadu_si128((__m128i*)&xor_res[i]);
            __m128i result = _mm_xor_si128(key_chunk, xor_chunk);
            _mm_storeu_si128((__m128i*)&xor_res[i], result);
            i += 16;
        }

        for (; i < key_len; i++) {
            xor_res[i] ^= key_local[i];
        }

        free(key_local);
        if (strlen(key) > size) {
            chunker_avx(key + size, size, xor_res);
        }
    }
#else
    void chunker_avx(char* key, int size, char* xor_res) {
        #if defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
        chunker_sse2(key, size, xor_res);
        #endif
    }
#endif

#if defined(__AVX2__)
    void chunker_avx2(char* key, int size, char* xor_res){
        if (key == NULL || xor_res == NULL || size <= 0) return;
        if (*key == '\0') return;

        int original_key_len = strlen(key);

        char* key_local = malloc(size + 1);
        if (key_local == NULL) return;
        memset(key_local, 0, size + 1);
        strncpy(key_local, key, size);

        int key_len = strlen(key);
        
        int i = 0;
        
        while (i <= key_len - 64) {
            __m256i key_chunk1 = _mm256_loadu_si256((__m256i*)&key_local[i]);
            __m256i key_chunk2 = _mm256_loadu_si256((__m256i*)&key_local[i + 32]);
            __m256i xor_chunk1 = _mm256_loadu_si256((__m256i*)&xor_res[i]);
            __m256i xor_chunk2 = _mm256_loadu_si256((__m256i*)&xor_res[i + 32]);
            
            __m256i result1 = _mm256_xor_si256(key_chunk1, xor_chunk1);
            __m256i result2 = _mm256_xor_si256(key_chunk2, xor_chunk2);
            
            _mm256_storeu_si256((__m256i*)&xor_res[i], result1);
            _mm256_storeu_si256((__m256i*)&xor_res[i + 32], result2);
            
            i += 64;
        }

        while (i <= key_len - 32) {
            __m256i key_chunk = _mm256_loadu_si256((__m256i*)&key_local[i]);
            __m256i xor_chunk = _mm256_loadu_si256((__m256i*)&xor_res[i]);
            __m256i result = _mm256_xor_si256(key_chunk, xor_chunk);
            _mm256_storeu_si256((__m256i*)&xor_res[i], result);
            i += 32;
        }
        
        while (i <= key_len - 16) {
            __m128i key_chunk = _mm_loadu_si128((__m128i*)&key_local[i]);
            __m128i xor_chunk = _mm_loadu_si128((__m128i*)&xor_res[i]);
            __m128i result = _mm_xor_si128(key_chunk, xor_chunk);
            _mm_storeu_si128((__m128i*)&xor_res[i], result);
            i += 16;
        }
        
        for (; i < key_len; i++) {
            xor_res[i] ^= key_local[i];
        }

        free(key_local);
        if (strlen(key) > size) {
            chunker_avx2(key + size, size, xor_res);
        }
    }
#else
    // Fallback for systems without AVX2
    void chunker_avx2(char* key, int size, char* xor_res) {
        // Forward to AVX implementation if available
        #if defined(__AVX__)
        chunker_avx(key, size, xor_res);
        #elif defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
        chunker_sse2(key, size, xor_res);
        #endif
    }
#endif
