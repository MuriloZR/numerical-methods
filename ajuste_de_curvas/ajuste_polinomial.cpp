#include <cmath>
#include <vector>
#include <iostream>
#include "../sistemas_lineares/sistemas_lineares.hpp"

template<typename T>
Matriz<T> build_pow_matrix(const tabela<T> &table, int degree) {
    Matriz<T> res(degree+1);

    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            for (int k = 0; k < table.x.size(); k++) {
                res(i, j) += std::pow(table.x[k], 2*(degree)-i-j);
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
auto polyfit(const tabela<T> &table, int degree) {
    Matriz<T> mat = build_pow_matrix(table, degree);
    std::vector<T> res = build_res_vector(table, degree);
    Sistema<T> M(degree+1);
    M.a = mat;
    M.b = res;
    return M.solve_gauss();
}

int main() {
    int n;
    std::cin >> n;

    tabela<double> T(n);

    for (int i{}; i < n; i++) {
        std::cin >> T.x[i] >> T.fx[i];
    }

    auto v = polyfit(T, 1);

    print_vetor(v);
}
