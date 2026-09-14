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

    [[nodiscard]]
    sistema<T> eliminacao_gauss() {
        sistema<T> ret {*this};
        for (int j = 0; j < ret.n-1; j++) {
            if (ret.a[j][j] == 0) {
                auto start_it = ret.a.begin() + j + 1;
                auto prox_n_nulo = std::find_if(start_it, ret.a.end(), [&j](const std::vector<T> &x){ return x[j] != 0; });
                if (prox_n_nulo != ret.a.end()) {
                    auto idx = std::distance(ret.a.begin(), prox_n_nulo);
                    std::swap(ret.a[j], ret.a[idx]);
                    std::swap(ret.b[j], ret.b[idx]);
                }
            }

            for (int i = j+1; i < ret.n; i++) {
                double m = ret.a[i][j]/ret.a[j][j];
                for (int k = j; k < ret.n; k++) {
                    ret.a[i][k] -= m*ret.a[j][k];
                }
                ret.b[i] -= m*ret.b[j];
            }
        }

        return ret;
    }

    [[nodiscard("Altera U in-place e retorna L")]]
    std::vector<std::vector<T>> eliminacao_gauss(std::vector<std::vector<T>> &U) {
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

    [[nodiscard("Solução de Ly = B")]]
    std::vector<T> find_y(const std::vector<std::vector<T>> &L, const std::vector<T> &B) {
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

    [[nodiscard("Solução de Ux = y")]]
    std::vector<T> find_x(const std::vector<std::vector<T>> &U, const std::vector<T> &y) {
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

    [[nodiscard("Solução do sistema linear")]]
    std::vector<T> solve_gauss() {
        auto A = eliminacao_gauss();
        return find_x(A.a, A.b);
    }

    [[nodiscard("Solução do Sistema")]]
    std::vector<T> solve_fatoracao_LU() {
        auto U = a;
        auto L = eliminacao_gauss(U);
        auto y = find_y(L, b);
        return find_x(U, y);
    }
};

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

    auto x = A.solve_gauss();
    A.print_sistema();
    print_solucao(x);

    x = A.solve_fatoracao_LU();
    A.print_sistema();
    print_solucao(x);
}