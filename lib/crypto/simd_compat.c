#include "../include/crypto/simd_compat.h"
#include <stdio.h>

#if defined(_MSC_VER) // Windows with Visual Studio
    #include <intrin.h>
    
    void init_cpu_features(CPUFeatures* features) {
        // Default to no features
        features->has_sse2 = 0;
        features->has_sse4_1 = 0;
        features->has_avx = 0;
        features->has_avx2 = 0;
        
        int cpu_info[4] = {0};
        
        // Get basic features (EAX=1)
        __cpuid(cpu_info, 1);
        
        // Check EDX register for SSE2 (bit 26)
        features->has_sse2 = (cpu_info[3] & (1 << 26)) != 0;
        
        // Check ECX register for SSE4.1 (bit 19) and AVX (bit 28)
        features->has_sse4_1 = (cpu_info[2] & (1 << 19)) != 0;
        features->has_avx = (cpu_info[2] & (1 << 28)) != 0;
        
        // Check for AVX2 which requires a different CPUID leaf
        if (features->has_avx) {
            __cpuid(cpu_info, 7);
            features->has_avx2 = (cpu_info[1] & (1 << 5)) != 0;
        }
    }
    
#elif defined(__GNUC__) // GCC/Clang on Linux/Mac
    #if defined(__x86_64__) || defined(__i386__)
        #include <cpuid.h>
        
        void init_cpu_features(CPUFeatures* features) {
            // Default to no features
            features->has_sse2 = 0;
            features->has_sse4_1 = 0;
            features->has_avx = 0;
            features->has_avx2 = 0;
            
            unsigned int eax, ebx, ecx, edx;
            
            // Get basic features (EAX=1)
            if (__get_cpuid(1, &eax, &ebx, &ecx, &edx)) {
                features->has_sse2 = (edx & (1 << 26)) != 0;
                features->has_sse4_1 = (ecx & (1 << 19)) != 0;
                features->has_avx = (ecx & (1 << 28)) != 0;
                
                // Check for AVX2
                if (features->has_avx) {
                    if (__get_cpuid_max(0, NULL) >= 7) {
                        __cpuid_count(7, 0, eax, ebx, ecx, edx);
                        features->has_avx2 = (ebx & (1 << 5)) != 0;
                    }
                }
            }
        }
    #else
        // Non-x86 architecture
        void init_cpu_features(CPUFeatures* features) {
            features->has_sse2 = 0;
            features->has_sse4_1 = 0;
            features->has_avx = 0;
            features->has_avx2 = 0;
            printf("CPU feature detection not supported on this architecture\n");
        }
    #endif
#else
    // Fallback for unknown compilers
    void init_cpu_features(CPUFeatures* features) {
        features->has_sse2 = 0;
        features->has_sse4_1 = 0;
        features->has_avx = 0;
        features->has_avx2 = 0;
        printf("CPU feature detection not supported with this compiler\n");
    }
#endif