#include "../codigos_auxiliares/custom_structs.hpp"
#include <cmath>

template<typename Fn, typename T>
requires std::invocable<Fn, T>
auto integrate(Fn &&f, T a, T b, uint32_t n = 100'000) -> decltype(f(a)) {
    T h = (b - a) / static_cast<T>(n);
    T sum = (f(a) + f(b)) / static_cast<T>(2);
    for (uint32_t i = 1; i < n; i++) sum += f(a + static_cast<T>(i) * h);
    return sum * h;
}


template<typename Fn, typename T>
auto integrate(const tabela<T> &table) {
    // TODO
}

int main() {
    auto f = []<typename T>(T x) { return std::exp(-(x*x)); };
    auto v = integrate(f, 0.0, 1.0);
    std::print("{}\n", v);
}