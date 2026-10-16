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
            C[i][j] = ( i == j ? 0 : -s.a[i][j]/s.a[i][i] );
        }
        g[i] = s.b[i]/s.a[i][i];
    }

    return std::make_pair(C, g);
}

template<typename T>
[[nodiscard]]
bool verify_convergence(const std::vector<std::vector<T>> &C) {
    std::vector<T> alpha(C.size(), 0), gamma(C.size(), 0);

    for (int i {}; i < alpha.size(); i++) {
        for (int j {}; j < alpha.size(); j++) {
            alpha[i] += std::abs(C[i][j]);
            gamma[j] += std::abs(C[i][j]);
        }
    }

    auto c1 = *std::ranges::max_element(alpha);
    auto c2 = *std::ranges::max_element(gamma);

    if (c1 < 1 || c2 < 1) return true;
    else return false;
}

template<typename T>
[[nodiscard]]
auto mult_CX(const std::vector<std::vector<T>> &C, std::vector<T> &X) {
    std::vector<T> res(X.size(), 0);
    for (int i {}; i < C.size(); i++) {
        for (int j {}; j < X.size(); j++) {
            res[i] += C[i][j] * X[j];
        }
    }

    return res;
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
    for (int i {}; i < X1.size(); i++) res += std::abs(X1[i] - X0[i]);
    return res;
}

template<typename T>
[[nodiscard]]
auto norma2(const std::vector<T> &X1, const std::vector<T> &X0) {
    T res {};
    for (int i {}; i < X1.size(); i++) res += (X1[i] - X0[i])*(X1[i] - X0[i]);
    return std::sqrt(res);
}

template<typename T>
[[nodiscard]]
auto norma3(const std::vector<T> &X1, const std::vector<T> &X0) {
    T res {};
    for (int i {}; i < X1.size(); i++) res = (std::abs(X1[i] - X0[i]) > res ? std::abs(X1[i] - X0[i]) : res);
    return res;
}

template<typename T>
[[nodiscard]]
auto solve_CG(const std::vector<std::vector<T>> &C, std::vector<T> &X, const std::vector<T> &g) {
    T eps{ std::numeric_limits<T>::epsilon() };
    int32_t iter{}, MAX_ITER{1'000'000};
    std::vector<T> X1(X.size());
    do {
        X1 = mult_CX(C, X);
        sum_g(X, g);
        X = X1;
        iter++;
    } while (iter < MAX_ITER && norma1(X1, X) > eps && norma2(X1, X) > eps && norma3(X1, X) > eps);
    return X1;
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

    auto [C, g] = build_CG(A);
    // A.print_sistema();

    bool converge = verify_convergence(C);

    if (converge) {
        auto sol = solve_CG(C, A.b, g);
        print_solucao(sol);
    }
    else {
        std::print("Um dos testes de convergência falhou\n");
        return 0;
    }
}