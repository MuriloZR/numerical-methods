#include <format>
#include <iostream>
#include <cstdint>
#include <print>
#include <cmath>

int main() {
    constexpr bool user_input{false};

    constexpr auto f = []<typename T>(T x) {return  std::exp(x) - std::cos(x) - 2.0;};

    // constexpr auto erro = [](double x0, double x1) {return std::abs(x1 - x0)/std::abs(x1);};
    constexpr auto erro = [f](double x0, double x1) {return std::abs(f(x1) - f(x0));};

    constexpr auto prox_ponto = [f](double x0, double x1) {return x1 - (f(x1)*(x1 - x0))/(f(x1) - f(x0));};

    double
        x0{1},
        x1{2},
        x2{},
        epsilon{std::numeric_limits<double>::epsilon()};

    int32_t
        iter{},
        MAX_ITER {1'000'000};


    if constexpr (user_input) {
        std::print("Insira dois pontos: ");
        std::cin >> x0 >> x1;
        while (std::abs(x1 - x0) < std::numeric_limits<double>::epsilon()) {
            std::print(std::cerr, "Os pontos {} e {} são iguais, escolha outros pontos: ", x0, x1);
            std::cin >> x0 >> x1;
        }
        x2 = prox_ponto(x0, x1);

        std::print("Insira o valor da tolerância: ");
        std::cin >> epsilon;

        std::print("Insira o máximo de iterações que o método deve fazer");
        std::cin >> MAX_ITER;
        while (MAX_ITER <= 0) {
            std::print(std::cerr, "Números menores ou iguais a 0, ou maiores que {} não são válidos. Insira um número de iterações válido: ", std::numeric_limits<int32_t>::max());
        }
    }

    while (erro(x0, x1) > epsilon && iter < MAX_ITER) {
        x0 = x1;
        x1 = x2;
        x2 = prox_ponto(x0, x1);
        iter++;
    }

    if (iter == MAX_ITER) {
        std::print("Número máximo de iterações atingido. Últimos valores computados:\n");
        std::print("Valor da função: {}    x0: {}    x1: {}    x2: {}\n", f(x2), x0, x1, x2);
    }
    else {
        std::print("Raiz da função: {}\nValor da função no ponto: {}\nn° iterações: {}\n", x1, f(x1), iter);
    }
}
