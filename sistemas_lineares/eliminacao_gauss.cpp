#include <vector>
#include <iostream>
#include <print>

template<typename T>
struct matriz {
    int rows, cols;
    std::pmr::vector<T> m;

    enum InputType {
        Default,
        User
    };

    matriz(int m, int n) : rows(m), cols(n), m(std::pmr::vector<T>(m*(n+1))){}

    void print() {
        for (int i{}; i < rows; i++) {
            for (int j{}; j < cols; j++) {
                std::print("{} ", m[i*cols + j]);
            }
            std::print("\n");
        }
    }

    void fill() {
        for (auto &i : m) {
            std::cin >> i;
        }
    }
};

void eliminacao_gauss(matriz<double> &A) {
    for (int j = 0; j < A.cols-1; j++) {
        for (int i = j+1; i < A.rows; i++) {
            double m = A.m[i * A.cols + j]/A.m[j * A.cols + j];
            for (int k = j; k <= A.cols; k++) {
                A.m[i * A.cols + k] -= m*A.m[j * A.cols + k];
            }
        }
    }
}

int main() {
    std::size_t n;
    std::cin >> n;
    matriz<double> A(n, n+1);
    A.fill();
    eliminacao_gauss(A);
    A.print();
}