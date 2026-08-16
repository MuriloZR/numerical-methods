#include <complex>
#include <format>
#include <iostream>
#include <cstdint>
#include <print>

int main() {
    constexpr bool user_input{false};

    constexpr auto f = []<typename T>(T x) {return  std::exp(x) - std::cos(x) - 2.0;};

    // constexpr auto df = [f](double x, double h = 1e-15) {return (f(x + h) - f(x))/h;}; // normal
    // constexpr auto df = [f](double x, double h = 1e-15) {return (f(x + h) - f(x - h))/(2.0*h);};
    constexpr auto df = [f](double x, double h = std::numeric_limits<double>::epsilon()) {return f(std::complex<double>(x, h)).imag()/h;};

    // constexpr auto erro = [](double x0, double x1) {return std::abs(x1 - x0)/std::abs(x1);};
    constexpr auto erro = [f](double x0, double x1) {return std::abs(f(x1) - f(x0));};

    constexpr auto prox_ponto = [f, df](double x0) {return x0 - f(x0)/df(x0);};

    double
        x0{1},
        x1{prox_ponto(x0)},
        epsilon{std::numeric_limits<double>::epsilon()};

    if constexpr (user_input) {
        std::print("Insira o ponto inicial: ");
        std::cin >> x0;
        while (df(x0) < 2.22e-16) {
            std::print(std::cerr, "Derivada no ponto {} é zero, escolha outro ponto: ", x0);
            std::cin >> x0;
        }
        std::print("Insira o valor da tolerância: ");
        std::cin >> epsilon;
        x1 = prox_ponto(x0);
    }

    int32_t iter{};
    constexpr int32_t MAX_ITER {1000};

    while (erro(x0, x1) > epsilon && iter < MAX_ITER) {
        x0 = x1;
        x1 = prox_ponto(x0);
        iter++;
    }

    std::print("Raiz da função: {}\nValor da função no ponto: {}\nn° iterações: {}\n", x1, f(x1), iter);
}