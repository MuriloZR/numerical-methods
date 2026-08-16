#include <iostream>
#include <print>

int main() {
    constexpr bool user_input{false};

    constexpr auto f = [](double x) {return x*x - 2.0;};
    constexpr auto pm = [](double a, double b) {return (a+b)/2.0;};

    double
        a{0},
        b{3},
        x{pm(a, b)},
        epsilon{std::numeric_limits<double>::epsilon()};

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
        x = pm(a, b);
    }

    int32_t iter{};

    while (b - a > epsilon) {
        if (f(a)*f(x) < 0.0)    b = x;
        else                    a = x;

        x = pm(a, b);
        iter++;
    }

    std::print("Raiz da função: {}\nValor da função no ponto: {}\nn° iterações: {}\n", x, f(x), iter);
}