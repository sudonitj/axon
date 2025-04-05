#ifndef PASSWORD_SIMD_H
#define PASSWORD_SIMD_H

void chunker_sse2(char* key, int size, char* xor_res);
void chunker_avx(char* key, int size, char* xor_res);
void chunker_avx2(char* key, int size, char* xor_res);

#endif // PASSWORD_SIMD_H