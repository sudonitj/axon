#ifndef DIFFUSION_SIMD_H
#define DIFFUSION_SIMD_H

void mix_columns_simd(char** state);
void init_diffusion_simd(void);

void mix_columns_sse2(char** state);
void mix_columns_avx(char** state); 
void mix_columns_avx2(char** state);

#endif // DIFFUSION_SIMD_H