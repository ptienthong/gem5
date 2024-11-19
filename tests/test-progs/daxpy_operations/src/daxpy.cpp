#include <iostream>
#include <vector>
#include <thread>
#include <omp.h>
#include <chrono>

const int VECTOR_SIZE = 1000000;
//const int NUM_THREADS = 4;
double a = 2.5; // Example scalar multiplier

// Function to perform daxpy on a segment of the vectors
void daxpy_segment(std::vector<double>& X, std::vector<double>& Y, int start, int end, double a) {
    #pragma omp parallel for
    for (int i = start; i < end; ++i) {
        Y[i] = a * X[i] + Y[i];
    }
}

int main() {
    std::vector<double> X(VECTOR_SIZE, 1.0); // Initialize X with 1.0
    std::vector<double> Y(VECTOR_SIZE, 2.0); // Initialize Y with 2.0

    // std::vector<std::thread> threads;
    // int segment_size = VECTOR_SIZE / NUM_THREADS;

    auto start = std::chrono::high_resolution_clock::now();
    // Launch threads
    // for (int t = 0; t < NUM_THREADS; ++t) {
    //     int start = t * segment_size;
    //     int end = (t == NUM_THREADS - 1) ? VECTOR_SIZE : start + segment_size;
    //     threads.emplace_back(std::thread(daxpy_segment, std::ref(X), std::ref(Y), start, end, a));
    // }

    daxpy_segment(std::ref(X), std::ref(Y), 0, VECTOR_SIZE, a);
    // for (auto& th : threads) {
    //     if (th.joinable()) {
    //         th.join();
    //     }
    // }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> parallel_time = end - start;
    std::cout << "Parallel Time: " << parallel_time.count() << " seconds" << std::endl;

    // for (int i = 0; i < Y.size(); i++){
    //     std::cout << Y[i] << ' ';
    // }
    // std::cout << '\n';
    std::cout << "DAXPY computation completed." << std::endl;
    return 0;
}
