#include <cmath>
#include <vector>
#include <print>
#include <algorithm>
#include <utility>

template<typename T>
struct Matriz {
    std::uint32_t rows, cols;
    std::vector<T> data;

    Matriz(std::size_t r, std::size_t c, T val = T{}):
        rows(r), cols(c), data(r * c, val)
    {}

    Matriz(std::size_t n, T val = T{}):
        Matriz(n, n, val)
    {}

    [[nodiscard]] inline T& operator()(std::size_t i, std::size_t j) noexcept {
        return data[i * cols + j];
    }

    [[nodiscard]] inline const T& operator()(std::size_t i, std::size_t j) const noexcept {
        return data[i * cols + j];
    }

    void print() const {
        for (std::size_t i = 0; i < rows; ++i) {
            for (std::size_t j = 0; j < cols; ++j) {
                std::print("{:10.4f} ", (*this)(i, j));
            }
            std::print("\n");
        }
    }
};

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
        Matriz<T> L(n, T{});
        Matriz<T> U(n, T{});

        for (std::size_t i = 0; i < n; ++i) {
            L(i, i) = 1;
            for (std::size_t j = i; j < n; ++j) {
                T sum{};
                for (std::size_t k = 0; k < i; ++k) sum += L(i, k) * U(k, j);
                U(i, j) = a(i, j) - sum;
            }
            for (std::size_t j = i + 1; j < n; ++j) {
                T sum{};
                for (std::size_t k = 0; k < i; ++k) sum += L(j, k) * U(k, i);
                L(j, i) = (a(j, i) - sum) / U(i, i);
            }
        }

        auto y = substituicao_direta(L, b);
        return retro_substituicao(U, y);
    }

    [[nodiscard]] std::vector<T> solve_gauss_jacobi(T tol = 1e-10, int max_iter = 1000) const {
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
                max_diff = std::max(max_diff, std::abs(x_new[i] - x[i]));
            }

            if (max_diff < tol) return x_new;
            std::swap(x, x_new);
        }
        return x;
    }

    [[nodiscard]] std::vector<T> solve_gauss_seidel(T tol = 1e-10, int max_iter = 1000) const {
        std::vector<T> x(n, T{0});

        for (int iter = 0; iter < max_iter; ++iter) {
            T max_diff = 0;
            for (std::size_t i = 0; i < n; ++i) {
                T sum = b[i];
                for (std::size_t j = 0; j < n; ++j) {
                    if (i != j) sum -= a(i, j) * x[j];
                }
                T novo_xi = sum / a(i, i);
                max_diff = std::max(max_diff, std::abs(novo_xi - x[i]));
                x[i] = novo_xi;
            }

            if (max_diff < tol) return x;
        }
        return x;
    }

    void print_matriz_ampliada() const {
        std::print("Matriz Ampliada do Sistema:\n");
        for (int i{}; i < n; i++) {
            for (int j{}; j < n; j++) {
                std::print("{} ", a.at(i, j));
            }
            std::print("| {}", b[i]);
            std::print("\n");
        }
    }

    void print_sistema() const {
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