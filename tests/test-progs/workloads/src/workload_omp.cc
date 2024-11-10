#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

int main() {
    const size_t N = 100000000; // Size of the vectors
    std::vector<double> A(N, 1.0); // Initialize vector A with 1.0
    std::vector<double> B(N, 2.0); // Initialize vector B with 2.0
    std::vector<double> C(N); // Result vector

    // Serial version
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
        C[i] = A[i] + B[i];
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> serial_time = end - start;
    std::cout << "Serial Time: " << serial_time.count() << " seconds" << std::endl;

    // Parallel version using OpenMP
    start = std::chrono::high_resolution_clock::now();
    #pragma omp parallel for
    for (size_t i = 0; i < N; ++i) {
        C[i] = A[i] + B[i];
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> parallel_time = end - start;
    std::cout << "Parallel Time: " << parallel_time.count() << " seconds" << std::endl;

    // Analyze speedup
    double speedup = serial_time.count() / parallel_time.count();
    std::cout << "Speedup: " << speedup << "x" << std::endl;

    return 0;
}
