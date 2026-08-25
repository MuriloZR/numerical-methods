#include <iostream>
#include <print>
#include <cmath>

int main() {
    constexpr bool user_input{false};

    constexpr auto f {[](double x) {return std::pow(x, 3) + x - 10.0;}};
    constexpr auto pm {[](double a, double b) {return (a+b)/2.0;}};

    constexpr auto erro_dominio {[](double a, double b) {return std::abs(b - a);}};
    constexpr auto erro_imagem {[f](double x) {return std::abs(f(x));}};

    double
        a{0},
        b{3},
        x{pm(a, b)},
        epsilon{std::numeric_limits<double>::epsilon()};

    int32_t
        iter{},
        MAX_ITER{1'000'000};
        // MAX_ITER{static_cast<int32_t>(std::ceil(std::log2(b - a) - std::log2(epsilon)))};

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
        x = pm(a, b);

        std::print("Insira o valor da tolerância: ");
        std::cin >> epsilon;

        std::print("Insira o máximo de iterações que o método deve fazer: ");
        std::cin >> MAX_ITER;
        while (MAX_ITER <= 0) {
            std::print(std::cerr, "Números menores ou iguais a 0, ou maiores que {} não são válidos. Insira um número de iterações válido: ", std::numeric_limits<int32_t>::max());
        }
    }

    while (erro_imagem(x) > epsilon && erro_dominio(a, b) > epsilon && iter < MAX_ITER) {
        if (f(a)*f(x) <= 0.0)    b = x;
        else                    a = x;

        x = pm(a, b);
        iter++;
    }

    if (std::isnan(f(x))) {
        std::print("\nO método divergiu para NaN\n");
    }
    else if (iter >= MAX_ITER) {
        std::print("\nNúmero máximo de iterações atingido. Últimos valores computados:\n");
        std::print("Valor da função: {}    a: {}    b: {}\n", f(x), a, b);
    }
    else {
        std::print("\nO método convergiu\n");
        std::print("Raiz da função: {}\nValor da função no ponto: {}\nn° iterações: {}\n", x, f(x), iter);
    }
}
