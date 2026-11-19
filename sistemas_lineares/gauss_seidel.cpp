#include <vector>
#include <iostream>
#include <print>
#include <algorithm>
#include <cmath>

template<typename T>
struct sistema {
    std::size_t n;
    std::vector<std::vector<T>> a;
    std::vector<T> b;

    sistema(size_t n)
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
void print_vetor(const std::vector<T> &x) {
    std::print("Solução:\nx = (");
    for (auto it = x.begin(); it != x.end(); it++) {
        std::print("{}", *it);
        if (it == x.end()-1) std::print(")\n");
        else std::print(", ");
    }
}

template<typename T>
[[nodiscard]]
auto build_CG(const sistema<T> &s) {
    std::vector<std::vector<T>> C(s.n, std::vector<T>(s.n));
    std::vector<T> g(s.n);

    for (size_t i = 0; i < s.n; i++) {
        for (size_t j = 0; j < s.n; j++) {
            C[i][j] = ( i == j ? 0 : -s.a[i][j]/s.a[i][i] );
        }
        g[i] = s.b[i]/s.a[i][i];
    }

    return std::make_pair(C, g);
}

template<typename T>
[[nodiscard]]
bool verify_convergence(const std::vector<std::vector<T>> &C) {
    if (C.empty()) { return false; }
    std::vector<T> beta(C.size(), T{0});
    for (std::size_t i = 0; i < C.size(); ++i) {
        T bi{};
        for (std::size_t j = 0; j < i; ++j) {
            bi += std::abs(C[i][j]) * beta[j];
        }
        for (std::size_t j = i + 1; j < C.size(); ++j) {
            bi += std::abs(C[i][j]);
        }
        beta[i] = bi;
    }

       return *std::max_element(beta.begin(), beta.end()) < static_cast<T>(1);
}

template<typename T>
void mult_CX_add_g(const std::vector<std::vector<T>> &C, std::vector<T> &X, const std::vector<T> &g) {
    std::vector<T> res(X.size(), 0);
    for (size_t i {}; i < C.size(); i++) {
        for (size_t j {}; j < X.size(); j++) {
            res[i] += C[i][j] * X[j];
        }
        X[i] = res[i] + g[i];
    }
}

template<typename T>
[[nodiscard]]
auto sum_g(std::vector<T> &X, const std::vector<T> &g) {
    for (int i {}; i < X.size(); i++) X[i] += g[i];
}

template<typename T>
[[nodiscard]]
auto norma1(const std::vector<T> &X1, const std::vector<T> &X0) {
    T res {};
    for (size_t i {}; i < X1.size(); i++) res += std::abs(X1[i] - X0[i]);
    return res;
}

template<typename T>
[[nodiscard]]
auto norma2(const std::vector<T> &X1, const std::vector<T> &X0) {
    T res {};
    for (size_t i {}; i < X1.size(); i++) res += (X1[i] - X0[i])*(X1[i] - X0[i]);
    return std::sqrt(res);
}

template<typename T>
[[nodiscard]]
auto norma3(const std::vector<T> &X1, const std::vector<T> &X0) {
    T res {};
    for (size_t i {}; i < X1.size(); i++) res = std::max(std::abs(X1[i] - X0[i]), res);
    return res;
}

template<typename T>
[[nodiscard]]
auto solve_CG(const std::vector<std::vector<T>> &C, std::vector<T> &X, const std::vector<T> &g,
              T eps = std::numeric_limits<T>::epsilon(), bool verbose = false) {
    int32_t iter{}, MAX_ITER{1'000'000};
    std::vector<T> X1 {X};
    while (iter < MAX_ITER) {
        mult_CX_add_g(C, X1, g);

        if (verbose) {
            print_vetor(X1);
        }

        if (norma1(X1, X) < eps || norma2(X1, X) < eps || norma3(X1, X) < eps) break;

        X = X1;
        iter++;
    }
    return X1;
}

int main() {
    std::size_t n;
    std::cin >> n;
    sistema<double> A(n);
    for (size_t i = 0; i < A.n; i++) {
        for (size_t j = 0; j < A.n; j++) {
            std::cin >> A.a[i][j];
        }
        std::cin >> A.b[i];
    }

    auto [C, g] = build_CG(A);
    // A.print_sistema();

    bool converge = verify_convergence(C);

    if (converge) {
        std::vector<double> x0(n, 0.0);
        auto sol = solve_CG(C, x0, g);
        print_vetor(sol);
    }
    else {
        std::print("Um dos testes de convergência falhou\n");
        return 0;
    }
}