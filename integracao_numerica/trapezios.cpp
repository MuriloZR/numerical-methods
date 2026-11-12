#include "../codigos_auxiliares/custom_structs.hpp"
#include <cmath>
#include <functional>
#include <iostream>

template<typename Fn, typename T>
requires std::invocable<Fn, T> && std::floating_point<T>
auto integrate(const Fn &f, T a, T b, uint32_t n = 100'000) {
    T h = (b - a) / static_cast<T>(n);
    T sum = (std::invoke(f, a) + std::invoke(f, b)) / static_cast<T>(2);
    for (size_t i = 1; i < n; i++) sum += std::invoke(f, a + static_cast<T>(i) * h);
    return sum * h;
}


template<typename T>
requires std::floating_point<T>
auto integrate(const tabela<T> &table) {
    T sum {};
    for (size_t i = 1; i < table.fx.size(); i++) {
        T dx = table.x[i] - table.x[i-1];
        sum += static_cast<T>(0.5) * (table.fx[i] + table.fx[i-1]) * dx;
    }
    return sum;
}

int main() {
    auto f = []<typename T>(T x) { return std::exp(-(x*x)); };
    auto v = integrate(f, 0.0, 1.0);

    int n; std::cin >> n;
    tabela<double> table(n);
    for (int i = 0; i < n; i++) std::cin >> table.x[i] >> table.fx[i];
    auto v2 = integrate(table);

    std::print("{}\n", v);
    std::print("{}\n", v2);
}