#include "../include/common/optimization.h"
#include "../include/crypto/simd_compat.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

OptimizationSettings g_opt_settings = {0};

void init_optimization_settings(OptimizationSettings* settings) {
    if (settings == NULL) return;

    memset(settings, 0, sizeof(OptimizationSettings));

    init_cpu_features(&settings->cpu_features);
    settings->current_level = OPT_LEVEL_NONE;
    settings->force_optimization_level = -1;

    const char* opt_env = getenv("AXON_OPT_LEVEL");
    if (opt_env != NULL) {
        if (strcmp(opt_env, "none") == 0) {
            settings->force_optimization_level = OPT_LEVEL_NONE;
        } else if (strcmp(opt_env, "sse2") == 0) {
            settings->force_optimization_level = OPT_LEVEL_SSE2;
        } else if (strcmp(opt_env, "avx") == 0) {
            settings->force_optimization_level = OPT_LEVEL_AVX;
        } else if (strcmp(opt_env, "avx2") == 0) {
            settings->force_optimization_level = OPT_LEVEL_AVX2;
        }
    }

    if (settings->force_optimization_level >= 0) {
        settings->current_level = settings->force_optimization_level;
    } else if (HAS_AVX2(&settings->cpu_features)) {
        settings->current_level = OPT_LEVEL_AVX2;
    } else if (HAS_AVX(&settings->cpu_features)) {
        settings->current_level = OPT_LEVEL_AVX;
    } else if (HAS_SSE2(&settings->cpu_features)) {
        settings->current_level = OPT_LEVEL_SSE2;
    } else {
        settings->current_level = OPT_LEVEL_NONE;
    }

    printf("Axon initialized with optimization level: %s\n", 
        get_optimization_level_name(settings->current_level));
}


void* get_optimal_implementation(void* original_func, 
                               void* sse2_func, 
                               void* avx_func,
                               void* avx2_func,
                               OptimizationSettings* settings) {
    switch (settings->current_level) {
        case OPT_LEVEL_AVX2:
            if (avx2_func) return avx2_func;            
        case OPT_LEVEL_AVX:
            if (avx_func) return avx_func;            
        case OPT_LEVEL_SSE2:
            if (sse2_func) return sse2_func;            
        case OPT_LEVEL_NONE:
        default:
            return original_func;
    }
}

const char* get_optimization_level_name(OptimizationLevel level) {
    switch (level) {
        case OPT_LEVEL_NONE: return "None (Original)";
        case OPT_LEVEL_SSE2: return "SSE2 (128-bit SIMD)";
        case OPT_LEVEL_AVX: return "AVX (256-bit SIMD)";
        case OPT_LEVEL_AVX2: return "AVX2 (256-bit SIMD Enhanced)";
        default: return "Unknown";
    }
}