#include "../codigos_auxiliares/custom_structs.hpp"
#include <cmath>
#include <functional>
#include <iostream>

template<typename Fn, typename T>
requires std::invocable<Fn, T> && std::floating_point<T>
auto integrate(const Fn &f, T a, T b, uint32_t n = 10'000) {
    if (n < 2) n = 2;
    if (n % 2 != 0) n++;

    T h {(b - a) / static_cast<T>(n)};
    T total_sum {(std::invoke(f, a) + std::invoke(f, b))};
    T even_sum {};
    T odd_sum {};

    for (uint32_t i = 1; i < n; i += 2) {
        odd_sum += std::invoke(f, a + static_cast<T>(i) * h);
    }
    for (uint32_t i = 2; i < n; i += 2) {
        even_sum += std::invoke(f, a + static_cast<T>(i) * h);
    }

    total_sum += odd_sum*(static_cast<T>(4));
    total_sum += even_sum*(static_cast<T>(2));
    return total_sum * (h/static_cast<T>(3));
}

template<typename T>
requires std::floating_point<T>
auto integrate(const tabela<T> &table) {
    size_t n = table.fx.size();
    if (n < 3 || (n-1)%2 != 0) throw std::invalid_argument("you need at least 3 points");

    T h = (table.x.back() - table.x.front()) / static_cast<T>(n - 1);
    T total_sum {(*table.fx.begin() + *(table.fx.end()-1))};
    T even_sum {};
    T odd_sum {};

    for (std::size_t i = 1; i < n - 1; i += 2) odd_sum += table.fx[i];
    for (std::size_t i = 2; i < n - 1; i += 2) even_sum += table.fx[i];

    total_sum += odd_sum*(static_cast<T>(4));
    total_sum += even_sum*(static_cast<T>(2));
    return total_sum * (h/static_cast<T>(3));
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