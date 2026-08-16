#include <iostream>
#include <print>
#include <algorithm>

int main() {
    constexpr bool user_input{false};

    constexpr auto f = [](double x) {return x*x - 2.0;};
    constexpr auto zero_reta = [f](double a, double b) {return (a*f(b) - b*f(a))/(f(b) - f(a));};

    double
        a{0},
        b{3},
        x{zero_reta(a, b)},
        epsilon{1e-15};

    auto handle_input = [&a, &b]() {
        std::cin >> a >> b;
        if (a > b) std::swap(a, b);
    };

    if constexpr (user_input) {
        std::print("Insira os pontos limite do intervalo [a, b]: ");
        handle_input();
        while (f(a)*f(b) > 0.0) {
            std::print(std::cerr, "A função não intercepta o ponto 0 no intervalo especificado, escolha outro intervalo: ");
            handle_input();
        }
        std::print("Insira o valor da tolerância: ");
        std::cin >> epsilon;
        x = zero_reta(a, b);
    }

    int32_t iter{};

    while (b - a > epsilon) {
        if (f(a)*f(x) < 0.0)    b = x;
        else                    a = x;

        x = zero_reta(a, b);
        iter++;
    }

    std::print("Raiz da função: {}\nValor da função no ponto: {}\nn° iterações: {}\n", x, f(x), iter);
}