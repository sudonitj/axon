#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include "../crypto/simd_compat.h"


typedef enum {
    OPT_LEVEL_NONE = 0,    // Original code, no optimizations
    OPT_LEVEL_SSE2 = 1,    // SSE2 optimizations (128-bit)
    OPT_LEVEL_AVX = 2,     // AVX optimizations (256-bit)
    OPT_LEVEL_AVX2 = 3     // AVX2 optimizations
} OptimizationLevel;

typedef struct {
    CPUFeatures cpu_features;
    OptimizationLevel current_level;
    int force_optimization_level;
} OptimizationSettings;

extern OptimizationSettings g_opt_settings;

void init_optimization_settings(OptimizationSettings* settings);

void* get_optimal_implementation(void* original_func, 
    void* sse2_func, 
    void* avx_func,
    void* avx2_func,
    OptimizationSettings* settings);

const char* get_optimization_level_name(OptimizationLevel level);


#endif //OPTIMIZATION_H