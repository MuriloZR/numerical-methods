#pragma once
#include <cmath>
#include <vector>
#include <print>
#include <algorithm>
#include <utility>
#include <limits>
#include "../codigos_auxiliares/custom_structs.hpp"

template<typename T>
class Sistema {
private:
    // Substituição retroativa: Ux = y (U é triangular superior)
    static std::vector<T> retro_substituicao(const Matriz<T>& U, const std::vector<T>& b) {
        std::size_t n = U.rows;
        std::vector<T> x(n);
        for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
            T sum{};
            for (std::size_t j = i + 1; j < n; ++j) {
                sum += U(i, j) * x[j];
            }
            x[i] = (b[i] - sum) / U(i, i);
        }
        return x;
    }

    // Substituição direta: Ly = b (L é triangular inferior com 1s na diagonal)
    static std::vector<T> substituicao_direta(const Matriz<T>& L, const std::vector<T>& b) {
        std::size_t n = L.rows;
        std::vector<T> y(n);
        for (std::size_t i = 0; i < n; ++i) {
            T sum{};
            for (std::size_t j = 0; j < i; ++j) {
                sum += L(i, j) * y[j];
            }
            y[i] = b[i] - sum;
        }
        return y;
    }

public:
    std::size_t n;
    Matriz<T> a;
    std::vector<T> b;

    Sistema(std::size_t n) : n(n), a(n), b(n) {}

    [[nodiscard]] std::vector<T> solve_gauss() const {
        if (n == 0) { return {}; }
        Matriz<T> A = a;
        std::vector<T> B = b;

        for (std::size_t j = 0; j < n - 1; ++j) {
            std::size_t pivo = j;
            T max_val = std::abs(A(j, j));
            for (std::size_t i = j + 1; i < n; ++i) {
                if (std::abs(A(i, j)) > max_val) {
                    max_val = std::abs(A(i, j));
                    pivo = i;
                }
            }

            if (max_val <= std::numeric_limits<T>::epsilon()) { return {}; }

            if (pivo != j) {
                for (std::size_t k = 0; k < n; ++k) std::swap(A(j, k), A(pivo, k));
                std::swap(B[j], B[pivo]);
            }

            for (std::size_t i = j + 1; i < n; ++i) {
                T m = A(i, j) / A(j, j);
                for (std::size_t k = j; k < n; ++k) {
                    A(i, k) -= m * A(j, k);
                }
                B[i] -= m * B[j];
            }
        }
        return retro_substituicao(A, B);
    }

    [[nodiscard]] std::vector<T> solve_fatoracao_LU() const {
        if (n == 0) return {};

        Matriz<T> L(n, T{});
        Matriz<T> U(n, T{});
        Matriz<T> A = a;
        std::vector<T> B = b;
        std::vector<T> col_vals(n);

        constexpr T eps = std::numeric_limits<T>::epsilon();

        for (std::size_t i = 0; i < n; ++i) {
            std::size_t pivot_row = i;
            T max_val = T{0};

            for (std::size_t row = i; row < n; ++row) {
                T sum{};
                for (std::size_t k = 0; k < i; ++k) {
                    sum += L(row, k) * U(k, i);
                }
                col_vals[row] = A(row, i) - sum;

                T abs_val = std::abs(col_vals[row]);
                if (abs_val > max_val) {
                    max_val = abs_val;
                    pivot_row = row;
                }
            }

            if (max_val <= eps) {
                return {};
            }

            if (pivot_row != i) {
                std::swap(col_vals[i], col_vals[pivot_row]);
                std::swap(B[i], B[pivot_row]);

                for (std::size_t col = i + 1; col < n; ++col) {
                    std::swap(A(i, col), A(pivot_row, col));
                }

                for (std::size_t col = 0; col < i; ++col) {
                    std::swap(L(i, col), L(pivot_row, col));
                }
            }

            L(i, i) = T{1};
            U(i, i) = col_vals[i];

            for (std::size_t j = i + 1; j < n; ++j) {
                T sum{};
                for (std::size_t k = 0; k < i; ++k) {
                    sum += L(i, k) * U(k, j);
                }
                U(i, j) = A(i, j) - sum;
            }

            for (std::size_t j = i + 1; j < n; ++j) {
                L(j, i) = col_vals[j] / U(i, i);
            }
        }

        auto y = substituicao_direta(L, B);
        return retro_substituicao(U, y);
    }

    [[nodiscard]] std::vector<T> solve_gauss_jacobi(T tol = 1e-10, int max_iter = 1000) const {
        if (n == 0) { return {}; }

        std::vector<T> x(n, T{0});
        std::vector<T> x_new(n, T{0});

        for (int iter = 0; iter < max_iter; ++iter) {
            T max_diff = 0;
            for (std::size_t i = 0; i < n; ++i) {
                T sum = b[i];
                for (std::size_t j = 0; j < n; ++j) {
                    if (i != j) sum -= a(i, j) * x[j];
                }
                x_new[i] = sum / a(i, i);
                if (std::isnan(x_new[i]) || std::isinf(x_new[i])) { return {}; }
                max_diff = std::max(max_diff, std::abs(x_new[i] - x[i]));
            }

            if (max_diff < tol) return x_new;
            std::swap(x, x_new);
        }
        return x;
    }

    [[nodiscard]] std::vector<T> solve_gauss_seidel(T tol = 1e-10, int max_iter = 1000) const {
        if (n == 0) { return {}; }

        std::vector<T> x(n, T{0});

        for (int iter = 0; iter < max_iter; ++iter) {
            T max_diff = 0;
            for (std::size_t i = 0; i < n; ++i) {
                T sum = b[i];
                for (std::size_t j = 0; j < n; ++j) {
                    if (i != j) sum -= a(i, j) * x[j];
                }
                T novo_xi = sum / a(i, i);
                if (std::isnan(novo_xi) || std::isinf(novo_xi)) { return {}; }
                max_diff = std::max(max_diff, std::abs(novo_xi - x[i]));
                x[i] = novo_xi;
            }

            if (max_diff < tol) return x;
        }
        return x;
    }

    void print_matriz_ampliada() const {
        if (n == 0) { return; }
        std::print("Matriz Ampliada do Sistema:\n");
        for (int i{}; i < n; i++) {
            for (int j{}; j < n; j++) {
                std::print("{} ", a(i, j));
            }
            std::print("| {}", b[i]);
            std::print("\n");
        }
    }

    void print_sistema() const {
        if (n == 0) { return; }
        std::print("Sistema:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                std::print("{}*x{}", a(i, j), j+1);
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
    std::print("Solução: (");
    for (std::size_t i = 0; i < x.size(); ++i) {
        std::print("{:.6f}{}", x[i], (i + 1 == x.size() ? ")\n" : ", "));
    }
}