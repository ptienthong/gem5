#include <iostream>
#include <vector>
#include <thread>

const int VECTOR_SIZE = 1000000;
double a = 2.5; // Example scalar multiplier

// Function to perform daxpy of the vectors
void daxpy_full(std::vector<double>& X, std::vector<double>& Y, int start, int end, double a) {
    for (int i = start; i < end; ++i) {
        Y[i] = a * X[i] + Y[i];
    }
}

int main() {
    std::vector<double> X(VECTOR_SIZE, 1.0); // Initialize X with 1.0
    std::vector<double> Y(VECTOR_SIZE, 2.0); // Initialize Y with 2.0

    daxpy_full(std::ref(X), std::ref(Y), 0, VECTOR_SIZE, a);

    std::cout << "DAXPY computation for single thread completed." << std::endl;
    return 0;
}
