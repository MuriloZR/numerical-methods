#include <iostream>
#include <print>
#include <vector>
#include <cmath>

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
std::vector<T> interpolate(tabela<T> table) {

}

int main() {
    int n;
    std::cin >> n;

    tabela<double> T(n);

    for (int i{}; i < n; i++) {
        std::cin >> T.x[i] >> T.fx[i];
    }
}