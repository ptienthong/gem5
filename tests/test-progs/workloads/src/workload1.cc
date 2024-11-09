#include <time.h>
#include <vector>
#include <stdio.h>

// use c++11 to compiler
const int size = 65;
const int num = 100;
template<typename T>
auto vectorAddition(const std::vector<T>& v1, const std::vector<T>& v2)  -> std::vector<T> {
    std::vector<T> vt(v1.size());
    for (int i=0; i < size; i++){
        vt[i] = v1[i] + v2[i];
    }

    return vt;
}

template<typename T>
auto vectorSubtraction(const std::vector<T>& v1, const std::vector<T>& v2)  -> std::vector<T> {
    std::vector<T> vt(v1.size());
    for (int i=0; i < size; i++){
        vt[i] = v1[i] - v2[i];
    }

    return vt;
}

template<typename T>
auto scalarMultiplication(const std::vector<T>& v)  -> std::vector<T> {
    std::vector<T> vt(v.size());
    for (int i=0; i < size; i++){
        vt[i] = num * v[i];
    }

    return vt;
}

int main(){

    std::vector<int> v1;
    std::vector<int> v2;
    // fill vector1
    for (int i=0; i < size; i++){
        v1.push_back(i);
    }
    // fill vector2
    for (int i=0; i < size; i++){
        v2.push_back(i);
    }

    // record start time
    auto start = clock();

    // vector addition
    auto va = vectorAddition(v1, v2);

    // vector subtraction
    //auto va = vectorSubtraction(v1, v2);

    // multiplication
    //auto va = scalarMultiplication(v1);

    // record end time
    auto end = clock();
    
    // calculate time taken
    auto time_taken = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    printf("Vector Operation took %f seconds \n", time_taken);

    return 0;
}
