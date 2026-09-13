#include <vector>
#include <iostream>
#include <print>

template<typename T>
struct matriz {
    int rows, cols;
    std::vector<std::vector<T>> m;

    matriz(int m, int n) : rows(m), cols(n), m(std::vector<std::vector<T>>(rows, std::vector<T>(cols))){}

    void print() {
        for (int i{}; i < rows; i++) {
            for (int j{}; j < cols; j++) {
                std::print("{} ", m[i][j]);
            }
            std::print("\n");
        }
    }
};

void eliminacao_gauss(matriz<double> &A) {
    for (int j = 0; j < A.cols-1; j++) {
        for (int i = j+1; i < A.rows; i++) {
            if (A.m[j][j] == 0) {
                auto prox_n_nulo = *std::ranges::find_if(A.m, [&j](const std::vector<double> &x){ return x[j] != 0; });
                std::swap(A.m[j], prox_n_nulo);
            }
            double m = A.m[i][j]/A.m[j][j];
            for (int k = j; k < A.cols; k++) {
                A.m[i][k] -= m*A.m[j][k];
            }
        }
    }
}

std::vector<double> solucao(const matriz<double> &A) {
    std::vector<double> sol;
    // TODO
    return sol;
}

int main() {
    std::size_t n;
    std::cin >> n;
    matriz<double> A(n, n+1);
    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < A.cols; j++) {
            std::cin >> A.m[i][j];
        }
    }
    eliminacao_gauss(A);
    A.print();
}