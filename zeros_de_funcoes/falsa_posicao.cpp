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
        epsilon{std::numeric_limits<double>::epsilon()};

    int32_t
        iter{},
        MAX_ITER{1'000'000};

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
        x = zero_reta(a, b);

        std::print("Insira o valor da tolerância: ");
        std::cin >> epsilon;

        std::print("Insira o máximo de iterações que o método deve fazer: ");
        std::cin >> MAX_ITER;
        while (MAX_ITER <= 0) {
            std::print(std::cerr, "Números menores ou iguais a 0, ou maiores que {} não são válidos. Insira um número de iterações válido: ", std::numeric_limits<int32_t>::max());
        }
    }

    while (b - a > epsilon && iter < MAX_ITER) {
        if (f(a)*f(x) < 0.0)    b = x;
        else                    a = x;

        x = zero_reta(a, b);
        iter++;
    }

    if (iter == MAX_ITER) {
        std::print("Número máximo de iterações atingido. Últimos valores computados:\n");
        std::print("Valor da função: {}    a: {}    b: {}\n", f(x), a, b);
    }
    else {
        std::print("\nO método convergiu\n");
        std::print("Raiz da função: {}\nValor da função no ponto: {}\nn° iterações: {}\n", x, f(x), iter);
    }
}
