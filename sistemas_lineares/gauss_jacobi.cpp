#include <vector>
#include <iostream>
#include <print>
#include <algorithm>

template<typename T>
struct sistema {
    std::size_t n;
    std::vector<std::vector<T>> a;
    std::vector<T> b;

    sistema(int n)
        : n(n), a(std::vector<std::vector<T>>(n, std::vector<T>(n))), b(std::vector<T>(n))
    {}

    void print_matriz_ampliada() const {
        std::print("Matriz Ampliada do Sistema:\n");
        for (int i{}; i < n; i++) {
            for (int j{}; j < n; j++) {
                std::print("{} ", a.at(i).at(j));
            }
            std::print("| {}", b[i]);
            std::print("\n");
        }
    }

    void print_sistema() const {
        std::print("Sistema:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                std::print("{}*x{}", a.at(i).at(j), j+1);
                if (j == n-1) std::print(" = ");
                else std::print(" + ");
            }
            std::print("{}", b.at(i));
            std::print("\n");
        }
    }
};

template<typename T>
[[nodiscard]]
auto build_CG(const sistema<T> &s) {
    std::vector<std::vector<T>> C(s.n, std::vector<T>(s.n));
    std::vector<T> g(s.n);

    for (int i = 0; i < s.n; i++) {
        for (int j = 1; j < s.n; j++) {
            C[i][j] = ( i == j ? 0 : -s(i, j)/s(i, i) );
        }
        g[i] = s.b[i]/s(i, i);
    }

    return C, g;
}

template<typename T>
[[nodiscard]]
auto mult_CX(const std::vector<std::vector<T>> &C, std::vector<T> &X) {

}

template<typename T>
[[nodiscard]]
auto solve_CG(const std::vector<std::vector<T>> &C, std::vector<T> &X, const std::vector<T> g) {
    T eps{ std::numeric_limits<T>::epsilon() };
    int32_t iter{}, MAX_ITER{1'000'000};
}

int main() {
    std::size_t n;
    std::cin >> n;
    sistema<double> A(n);
    for (int i = 0; i < A.n; i++) {
        for (int j = 0; j < A.n; j++) {
            std::cin >> A.a[i][j];
        }
        std::cin >> A.b[i];
    }

    A.print_sistema();
}