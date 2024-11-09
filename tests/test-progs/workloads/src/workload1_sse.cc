#include <xmmintrin.h>
#include <vector>
#include <stdio.h>
#include <chrono>  // For more precise timing

const int size = 65;
const int num = 100;

// SSE vector addition
template<typename T>
auto vectorAdditionSSE(const std::vector<T>& v1, const std::vector<T>& v2) -> std::vector<T> {
    std::vector<T> vt(v1.size());
    int i;

    // Process data in chunks of 4
    for (i = 0; i < size / 4 * 4; i += 4) {
        // Load 4 elements from v1 and v2 into SSE registers
        __m128 v1_vec = _mm_load_ps(reinterpret_cast<const float*>(&v1[i]));  // Using aligned load
        __m128 v2_vec = _mm_load_ps(reinterpret_cast<const float*>(&v2[i]));  // Using aligned load

        // Perform the addition (SSE adds 4 values in parallel)
        __m128 result = _mm_add_ps(v1_vec, v2_vec);

        // Store the result back into vt
        _mm_store_ps(reinterpret_cast<float*>(&vt[i]), result);  // Using aligned store
    }

    // Process remaining elements (if size is not a multiple of 4)
    for (; i < size; ++i) {
        vt[i] = v1[i] + v2[i];
    }

    return vt;
}

// SSE vector subtraction
template<typename T>
auto vectorSubtractionSSE(const std::vector<T>& v1, const std::vector<T>& v2) -> std::vector<T> {
    std::vector<T> vt(v1.size());
    int i;

    for (i = 0; i < size / 4 * 4; i += 4) {
        __m128 v1_vec = _mm_load_ps(reinterpret_cast<const float*>(&v1[i]));
        __m128 v2_vec = _mm_load_ps(reinterpret_cast<const float*>(&v2[i]));
        __m128 result = _mm_sub_ps(v1_vec, v2_vec);
        _mm_store_ps(reinterpret_cast<float*>(&vt[i]), result);
    }

    for (; i < size; ++i) {
        vt[i] = v1[i] - v2[i];
    }

    return vt;
}

// SSE scalar multiplication
template<typename T>
auto scalarMultiplicationSSE(const std::vector<T>& v) -> std::vector<T> {
    std::vector<T> vt(v.size());
    __m128 scalar = _mm_set1_ps(num);  // Set the scalar value to all 4 elements of the register
    int i;

    for (i = 0; i < size / 4 * 4; i += 4) {
        __m128 v_vec = _mm_load_ps(reinterpret_cast<const float*>(&v[i]));
        __m128 result = _mm_mul_ps(v_vec, scalar);
        _mm_store_ps(reinterpret_cast<float*>(&vt[i]), result);
    }

    for (; i < size; ++i) {
        vt[i] = num * v[i];
    }

    return vt;
}

int main() {
    // Using float vectors to ensure SSE operations work properly
    std::vector<float> v1(size);
    std::vector<float> v2(size);
    
    // Initialize vectors using a simple loop, no need for push_back
    for (int i = 0; i < size; ++i) {
        v1[i] = static_cast<float>(i); // Initialize v1 with values
        v2[i] = static_cast<float>(i); // Initialize v2 with values
    }

    // Start timing using high resolution clock
    auto start = std::chrono::high_resolution_clock::now();

    // Perform vector addition (can change to subtraction or multiplication as needed)
    auto va = vectorAdditionSSE(v1, v2);

    // Uncomment the operation you want to test
    // auto va = vectorSubtractionSSE(v1, v2);
    // auto va = scalarMultiplicationSSE(v1);

    // Stop timing
    auto end = std::chrono::high_resolution_clock::now();
    
    // Calculate and print time taken
    std::chrono::duration<double> time_taken = end - start;
    printf("Vector Operation took %f seconds\n", time_taken.count());

    return 0;
}
