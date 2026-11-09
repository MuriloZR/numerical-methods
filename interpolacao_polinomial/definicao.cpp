#include "../sistemas_lineares/sistemas_lineares.hpp"
#include <iostream>
#include <vector>
#include <cmath>

template<typename T>
struct tabela {
    std::vector<T> x;
    std::vector<T> fx;

    tabela(size_t n)
        : x(n, 0), fx(n, 0)
    {}

    tabela(const std::vector<T> &x, const std::vector<T> &y)
        : x(x), fx(y)
    {}
};

template<typename T>
Matriz<T> build_coef_mat(const tabela<T> &table) {
    size_t n = table.x.size();
    Matriz<T> ret(n);

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            ret(i, j) = std::pow(table.x[i], j);
        }
    }

    return ret;
}

template<typename T>
auto polyfit(const tabela<T> &table) {
    Matriz<T> mat = build_coef_mat(table);
    Sistema<T> M(table.x.size());
    M.a = mat;
    M.b = table.fx;
    return M.solve_gauss();
}

template<typename T>
auto evaluate(std::vector<T> poly, T x) {
    T res{};
    for (int i = 0; i < poly.size(); i++) {
        res += poly[i]*std::pow(x, i);
    }

    return res;
}

int main() {
    int n;
    std::cin >> n;

    tabela<double> T(n);

    for (size_t i = 0; i < n; i++) {
        std::cin >> T.x[i] >> T.fx[i];
    }

    auto v = polyfit(T);

    for (int i = 0; i < n; i++) {
        std::print("P(x) = {} f(x) = {}\n", evaluate(v, T.x[i]), T.fx[i]);
    }
}