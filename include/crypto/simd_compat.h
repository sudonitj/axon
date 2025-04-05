#ifndef SIMD_COMPAT_H
#define SIMD_COMPAT_H

typedef struct {
    int has_sse2;
    int has_sse4_1;
    int has_avx;
    int has_avx2;
} CPUFeatures;

void init_cpu_features(CPUFeatures* features);

#define HAS_SSE2(features) ((features)->has_sse2)
#define HAS_SSE4_1(features) ((features)->has_sse4_1)
#define HAS_AVX(features) ((features)->has_avx)
#define HAS_AVX2(features) ((features)->has_avx2)

#endif //SIMD_COMPAT_H