#include <iostream>
#include <vector>
#include <thread>

const int VECTOR_SIZE = 1000000;
const int NUM_THREADS = 4;
double a = 2.5; // Example scalar multiplier

// Function to perform daxpy on a segment of the vectors
void daxpy_segment(std::vector<double>& X, std::vector<double>& Y, int start, int end, double a) {
    for (int i = start; i < end; ++i) {
        Y[i] = a * X[i] + Y[i];
    }
}

int main() {
    std::vector<double> X(VECTOR_SIZE, 1.0); // Initialize X with 1.0
    std::vector<double> Y(VECTOR_SIZE, 2.0); // Initialize Y with 2.0

    std::vector<std::thread> threads;
    int segment_size = VECTOR_SIZE / NUM_THREADS;

    // Launch threads
    for (int t = 0; t < NUM_THREADS; ++t) {
        int start = t * segment_size;
        int end = (t == NUM_THREADS - 1) ? VECTOR_SIZE : (t + 1) * segment_size;
        threads.push_back(std::thread(daxpy_segment, std::ref(X), std::ref(Y), start, end, a));
    }

    // Join threads
    for (auto& th : threads) {
        th.join();
    }

    std::cout << "DAXPY computation completed." << std::endl;
    return 0;
}
