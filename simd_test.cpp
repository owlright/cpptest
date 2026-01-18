//// filepath: /home/sjh/cpptest/simd_test.cpp
#include <iostream>
#include <cstdlib>
#include <chrono>

#ifdef __AVX__
  #include <immintrin.h>
#else
  #warning No AVX support - will not compile
#endif

// 数组大小
const int SIZE = 1 << 20; // 约 1M 元素

// 对齐分配函数
float* aligned_alloc_array(size_t size, size_t alignment) {
    void* ptr = nullptr;
    if (posix_memalign(&ptr, alignment, size * sizeof(float)) != 0) {
        return nullptr;
    }
    return reinterpret_cast<float*>(ptr);
}

int main() {
    // 未对齐数组
    float* arr1 = new float[SIZE+1];
    float* arr2 = new float[SIZE+1];
    float* result_unaligned = new float[SIZE];

    // 对齐数组 (32字节对齐，适合 AVX)
    float* arr3 = aligned_alloc_array(SIZE, 32);
    float* arr4 = aligned_alloc_array(SIZE, 32);
    float* result_aligned = aligned_alloc_array(SIZE, 32);

    // 初始化数据
    for (int i = 0; i < SIZE; ++i) {
        arr1[i+1] = arr2[i+1] = arr3[i] = arr4[i] = 1.0f;
    }

    // 未对齐 SIMD 加法
    auto start1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < SIZE; i += 8) {
        __m256 a = _mm256_loadu_ps(&arr1[i+1]); // unaligned load
        __m256 b = _mm256_loadu_ps(&arr2[i+1]);
        __m256 c = _mm256_add_ps(a, b);
        _mm256_storeu_ps(&result_unaligned[i], c);
    }
    auto end1 = std::chrono::high_resolution_clock::now();

    // 对齐 SIMD 加法
    auto start2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < SIZE; i += 8) {
        __m256 a = _mm256_load_ps(&arr3[i]); // aligned load
        __m256 b = _mm256_load_ps(&arr4[i]);
        __m256 c = _mm256_add_ps(a, b);
        _mm256_store_ps(&result_aligned[i], c);
    }
    auto end2 = std::chrono::high_resolution_clock::now();

    // 输出耗时
    std::cout << "未对齐 SIMD 耗时: "
              << std::chrono::duration<double>(end1 - start1).count() << " 秒\n";
    std::cout << "对齐 SIMD 耗时: "
              << std::chrono::duration<double>(end2 - start2).count() << " 秒\n";

    // 释放内存
    delete[] arr1;
    delete[] arr2;
    delete[] result_unaligned;
    free(arr3);
    free(arr4);
    free(result_aligned);

    return 0;
}
