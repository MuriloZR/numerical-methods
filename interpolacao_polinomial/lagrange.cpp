#include "../codigos_auxiliares/custom_structs.hpp"
#include <iostream>
#include <print>

template<typename T>
auto evaluate(const tabela<T> &table, T x) {
    int n = table.fx.size();

    T res{};
    for (int i = 0; i < n; i++) {
        T Li {1};
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            Li *= (x - table.x[j])/(table.x[i] - table.x[j]);
        }
        res += Li*table.fx[i];
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

    for (int i = 0; i < n; i++) {
        std::print("P(x) = {} f(x) = {}\n", evaluate(T, T.x[i]), T.fx[i]);
    }
}