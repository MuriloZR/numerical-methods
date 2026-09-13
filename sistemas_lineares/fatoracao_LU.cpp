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
std::vector<std::vector<T>> eliminacao_gauss(std::vector<std::vector<T>> &U, int n) {
    std::vector<std::vector<T>> L(n, std::vector<T>(n, 0));

    L[n-1][n-1] = 1;
    for (int j = 0; j < n-1; j++) {
        L[j][j] = 1;
        if (U[j][j] == 0) {
            auto start_it = U.begin() + j + 1;
            auto prox_n_nulo = std::find_if(start_it, U.end(), [&j](const std::vector<T> &x){ return x[j] != 0; });
            if (prox_n_nulo != U.end()) {
                auto idx = std::distance(U.begin(), prox_n_nulo);
                std::swap(U[j], U[idx]);
            }
        }

        for (int i = j+1; i < n; i++) {
            double m = U[i][j]/U[j][j];
            for (int k = j; k < n; k++) {
                U[i][k] -= m*U[j][k];
            }
            L[i][j] = m;
        }
    }

    return L;
}

template<typename T>
[[nodiscard("Solução de Ly = B")]]
std::vector<T> find_y(std::vector<std::vector<T>> L, std::vector<T> B, int n) {
    std::vector<double> y(n);
    y[0] = B[0]/L[0][0];
    for (int i = 1; i < n; i++) {
        T sum{};
        for (int j = i-1; j >= 0; j--) {
            sum += L[i][j] * y[j];
        }
        y[i] = (B[i] - sum)/L[i][i];
    }
    return y;
}

template<typename T>
[[nodiscard("Solução do sistema linear")]]
std::vector<T> find_x(std::vector<std::vector<T>> U, std::vector<T> y, int n) {
    std::vector<double> x(n);
    x[x.size()-1] = y[n-1]/U[n-1][n-1];
    for (int i = n-2; i >= 0; i--) {
        T sum{};
        for (int j = i+1; j < n; j++) {
            sum += U[i][j] * x[j];
        }
        x[i] = (y[i] - sum)/U[i][i];
    }
    return x;
}

template<typename T>
void print_matriz(std::vector<std::vector<T>> m, int n) {
    for (int i{}; i < n; i++) {
        for (int j{}; j < n; j++) {
            std::print("{} ", m.at(i).at(j));
        }
        std::print("\n");
    }
}

template<typename T>
std::vector<std::vector<T>> mult_matriz(std::vector<std::vector<T>> A, std::vector<std::vector<T>> B) {
    std::vector<std::vector<T>> C(A.size(), std::vector<T>(A.size(), 0));
    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < A.size(); j++) {
            for (int k = 0; k < A.size(); k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
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

    auto L = eliminacao_gauss(A.a, A.n);
    auto y = find_y(L, A.b, A.n);
    auto x = find_x(A.a, y, A.n);
    print_solucao(x);
}