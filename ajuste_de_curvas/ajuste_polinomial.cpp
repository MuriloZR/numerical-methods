#include <cmath>
#include <vector>
#include <iostream>
#include <print>

template<typename T>
struct tabela {
    std::vector<T> x;
    std::vector<T> fx;

    tabela(size_t n)
        : x(std::vector<T>(n, 0)), fx(std::vector<T>(n, 0))
    {}

    tabela(const std::vector<T> &x, const std::vector<T> &y)
        : x(x), fx(y)
    {}
};

template<typename T>
struct matriz {
    int rows, cols;
    std::vector<std::vector<T>> m;

    matriz(std::vector<std::vector<T>> v)
        : m(v), rows(v.size()), cols(v[0].size())
    {}

    matriz(size_t size, T val):
        m(std::vector<std::vector<T>>(size, std::vector<T>(size, val))),
        rows(size),
        cols(size)
    {}

    matriz(size_t rows, size_t cols, T val):
        m(std::vector<std::vector<T>>(rows, std::vector<T>(cols, val))),
        rows(rows),
        cols(cols)
    {}

    [[nodiscard]]
    std::vector<T> operator*(const std::vector<T>& v) const {
        std::vector<T> res(this->rows, 0);
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                res[i] += this->m[i][j] * v[j];
            }
        }
        return res;
    }

    [[nodiscard]]
    matriz<T> operator*(const matriz& other) const {
        matriz<T> res(rows, other.cols, 0);
        for (int i = 0; i < this->rows; i++) {
            for (int k = 0; k < this->cols; k++) {
                for (int j = 0; j < other.cols; j++) {
                    res[i][j] += this->m[i][k] * other.m[k][j];
                }
            }
        }
        return res;
    }

    void print() const {
        for (int i{}; i < rows; i++) {
            for (int j{}; j < cols; j++) {
                std::print("{} ", m.at(i).at(j));
            }
            std::print("\n");
        }
    }
};

template<typename T>
matriz<T> build_pow_matrix(const tabela<T> &table, int degree) {
    matriz<T> res(degree+1, 0);

    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            for (int k = 0; k < table.x.size(); k++) {
                res.m[i][j] += std::pow(table.x[k], 2*(degree)-i-j);
            }
        }
    }

    return res;
}

template<typename T>
std::vector<T> build_res_vector(const tabela<T> &table, int degree) {
    std::vector<T> res(degree+1, 0);

    for (int i = 0; i < res.size(); i++) {
        for (int j = 0; j < table.x.size(); j++) {
            res[i] += table.fx[j]*std::pow(table.x[j], (degree)-i);
        }
    }

    return res;
}

template<typename T>
void print_vetor(const std::vector<T> &x) {
    for (auto it = x.begin(); it != x.end(); it++) {
        std::print("{} ", *it);
    }
    std::print("\n");
}

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
auto solve(const tabela<T> &table, int degree) {
    matriz<T> mat = build_pow_matrix(table, degree);
    std::vector<T> res = build_res_vector(table, degree);
    sistema<T> M(degree+1);
    M.a = mat.m;
    M.b = res;
    eliminacao_gauss(M);
    return find_solucao(M);
}

int main() {
    int n;
    std::cin >> n;

    tabela<double> T(n);

    for (int i{}; i < n; i++) {
        std::cin >> T.x[i] >> T.fx[i];
    }

    auto v = solve(T, 1);

    print_vetor(v);
}