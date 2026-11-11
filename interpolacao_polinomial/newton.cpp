#include <vector>
#include <iostream>
#include <print>
#include "../codigos_auxiliares/custom_structs.hpp"

template<typename T>
auto generate_div_diff_table(const tabela<T> &table) {
    std::vector<std::vector<T>> diferencas_divididas(table.fx.size());
    T res{};
    for (int i = 0; i < table.fx.size(); i++) {
        diferencas_divididas[0].push_back(table.fx[i]);
    }
    for (int i = 1; i < table.fx.size(); i++) {
        for (int j = 0; j < diferencas_divididas[i-1].size()-1; j++) {
            diferencas_divididas[i].push_back((diferencas_divididas[i-1][j+1] - diferencas_divididas[i-1][j])/(table.x[j+i] - table.x[j]));
        }
    }

    return diferencas_divididas;
}

template<typename T>
auto evaluate(const tabela<T> &table, const std::vector<std::vector<T>> &div_diffs, T x) {
    T res{div_diffs[0][0]};
    for (int i = 1; i < table.fx.size(); i++) {
        T tmp {1};
        for (int j = 0; j < i; j++) {
            tmp *= (x - table.x[j]);
        }
        res += div_diffs[i][0] * tmp;
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

    auto div_diffs = generate_div_diff_table(T);

    for (int i = 0; i < n; i++) {
        std::print("P(x) = {} f(x) = {}\n", evaluate(T, div_diffs, T.x[i]), T.fx[i]);
    }
}