#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// Vector size and scalar multiplier
#define VECTOR_SIZE 1000
#define NUM_THREADS 8
double a = 2.5; // Example scalar multiplier

// Vectors X and Y
double X[VECTOR_SIZE];
double Y[VECTOR_SIZE];

// DAXPY operation performed by each thread
void* daxpy_thread(void* arg) {
    int thread_id = *(int*)arg;
    int start = (VECTOR_SIZE / NUM_THREADS) * thread_id;
    int end = (thread_id == NUM_THREADS - 1) ? VECTOR_SIZE : start + (VECTOR_SIZE / NUM_THREADS);

    for (int i = start; i < end; ++i) {
        Y[i] = a * X[i] + Y[i];
    }

    return NULL;
}

int main() {
    // Initialize vectors X and Y
    for (int i = 0; i < VECTOR_SIZE; ++i) {
        X[i] = 1.0; // Initialize X with 1.0
        Y[i] = 2.0; // Initialize Y with 2.0
    }

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Create and join threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, daxpy_thread, &thread_ids[i])) {
            perror("pthread_create");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        if (pthread_join(threads[i], NULL)) {
            perror("pthread_join");
            exit(1);
        }
    }

    // Print a few elements of Y to verify correctness
    printf("DAXPY computation completed. Sample results:\n");
    for (int i = 0; i < 10; ++i) {
        printf("Y[%d] = %f\n", i, Y[i]);
    }

    return 0;
}
