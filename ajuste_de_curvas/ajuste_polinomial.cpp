#include <vector>
#include <iostream>

template<typename T>
struct tabela {
    std::vector<T> x;
    std::vector<T> fx;
};

int main() {
    int n;
    tabela<double> T;

    for (int i{}; i < n; i++) {
        std::cin >> T.x[i] >> T.fx[i];
    }
}