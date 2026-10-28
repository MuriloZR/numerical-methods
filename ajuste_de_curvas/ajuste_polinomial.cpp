#include <vector>
#include <iostream>

template<typename T>
struct tabela {
    std::vector<T> x;
    std::vector<T> fx;

    tabela<T>(int n)
        : x(std::vector<T>(n, 0)), fx(std::vector<T>(n, 0))
    {}

    tabela<T>(const std::vector<T> &x, const std::vector<T> &y)
        : x(x), fx(y)
    {}
};

template<typename T>
struct matriz {
    int rows, cols;
    std::vector<std::vector<T>> m;

    matriz<T>(std::vector<std::vector<T>> v)
        : m(v), rows(v.size()), cols(v[0].size())
    {}

    matriz<T>(int size, int val):
        m(std::vector<std::vector<T>>(size, std::vector<T>(size, val))),
        rows(size),
        cols(size)
    {}

    matriz<T>(int rows, int cols, int val):
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
};

template<typename T>
matriz<T> build_pow_matrix(const tabela<T> &table, int degree) {
    matriz<T> res(degree+1, 0);

    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            for (int k = 0; k < table.x.size(); k++) {
                res[i][j] += pow(table.x[k], 2*(degree)-i-j);
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
            res[i] += table.fx[j]*pow(table.x[j], (degree)-i);
        }
    }

    return res;
}

template<typename T>
std::vector<T> solve(const tabela<T> &table, int degree) {
    matriz<T> mat = build_pow_matrix(table, degree);
    std::vector<T> res = build_res_vector(table, degree);
}

int main() {
    int n;
    std::cin >> n;

    tabela<double> T(n);

    for (int i{}; i < n; i++) {
        std::cin >> T.x[i] >> T.fx[i];
    }
}