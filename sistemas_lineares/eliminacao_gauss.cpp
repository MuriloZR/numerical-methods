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
void eliminacao_gauss(sistema<T> &A) {
    for (int j = 0; j < A.n-1; j++) {
        if (A.a[j][j] == 0) {
            auto start_it = A.a.begin() + j + 1;
            auto prox_n_nulo = std::find_if(start_it, A.a.end(), [&j](const std::vector<T> &x){ return x[j] != 0; });
            if (prox_n_nulo != A.a.end()) {
                auto idx = std::distance(A.a.begin(), prox_n_nulo);
                std::swap(A.a[j], A.a[idx]);
                std::swap(A.b[j], A.b[idx]);
            }
        }

        for (int i = j+1; i < A.n; i++) {
            double m = A.a[i][j]/A.a[j][j];
            for (int k = j; k < A.n; k++) {
                A.a[i][k] -= m*A.a[j][k];
            }
            A.b[i] -= m*A.b[j];
        }
    }
}

template<typename T>
[[nodiscard("Solução do sistema linear")]]
std::vector<T> find_solucao(const sistema<T> &A) {
    std::vector<double> sol(A.n);
    sol[sol.size()-1] = A.b[A.n-1]/A.a[A.n-1][A.n-1];
    for (int i = A.n-2; i >= 0; i--) {
        T sum{};
        for (int j = i+1; j < A.n; j++) {
            sum += A.a[i][j] * sol[j];
        }
        sol[i] = (A.b[i] - sum)/A.a[i][i];
    }
    return sol;
}

template<typename T>
[[nodiscard("Solução do sistema linear")]]
std::vector<T> solve(const sistema<T> &A) {
    sistema<T> M {A};
    eliminacao_gauss(M);
    return find_solucao(M);
}

template<typename T>
void print_solucao(const std::vector<T> &x) {
    std::print("Solução:\nx = (");
    for (auto it = x.begin(); it != x.end(); it++) {
        std::print("{}", *it);
        if (it == x.end()-1) std::print(")\n");
        else std::print(", ");
    }
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
    auto x = solve(A);
    A.print_sistema();
    A.print_matriz_ampliada();
    print_solucao(x);
}