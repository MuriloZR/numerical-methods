#include <iostream>
#include <print>
#include <cmath>
#include <concepts>
#include <limits>
#include <complex>

enum class Status {
    Success,
    MaxIterationsReached,
    InvalidInterval,
    DivergedToNaN
};

struct Result {
    double root;
    double f_root;
    int32_t iterations;
    Status status;
};

template <typename Func>
requires std::invocable<Func, double>
auto bissessao(Func&& f, double a, double b,
                double epsilon = std::numeric_limits<double>::epsilon(),
                int32_t max_iter = 1000) -> Result
{
    auto pm {[&a, &b](){
        return a+(b-a)/2.0;}
    };

    double
        fa {f(a)},
        fb {f(b)},
        tol_abs {epsilon},
        tol_rel {epsilon};

    if (fa * fb > 0.0) return { 0.0, 0.0, 0, Status::InvalidInterval };

    double x {pm()};
    double fx {f(x)};

    for (int32_t iter {0}; iter < max_iter; iter++) {
        double current_tol {tol_abs + tol_rel * std::max(std::abs(a), std::abs(b))};

        if (std::abs(fx) <= tol_abs || std::abs(b - a) < current_tol) {
            return { x, fx, iter, Status::Success };
        }

        if (std::isnan(fx) || std::isinf(fx)) return { x, fx, iter, Status::DivergedToNaN };

        if (fa * fx <= 0.0) {
            b = x; fb = fx;
        }
        else {
            a = x; fa = fx;
        }

        x = pm();
        fx = f(x);
    }

    return { x, fx, max_iter, Status::MaxIterationsReached };
}

template <typename Func>
requires std::invocable<Func, double>
auto falsa_posicao(Func&& f, double a, double b,
                double epsilon = std::numeric_limits<double>::epsilon(),
                int32_t max_iter = 1000) -> Result
{
    double
        fa {f(a)},
        fb {f(b)},
        x {},
        fx {},
        tol_abs = epsilon,
        tol_rel = epsilon;

    auto zero_reta {[&a, &fa, &b, &fb](){return a - fa * (b-a)/(fb-fa);}};

    x = zero_reta();
    fx = f(x);

    if (fa * fb > 0.0) return { 0.0, 0.0, 0, Status::InvalidInterval };

    for (int32_t iter = 0; iter < max_iter; ++iter) {
        if (std::abs(fb-fa) < std::numeric_limits<double>::epsilon()) {
            return { a, fa, iter, Status::DivergedToNaN };
        }

        double current_tol = tol_abs + tol_rel * std::max(std::abs(a), std::abs(b));
        if (std::abs(fx) <= tol_abs || std::abs(b - a) < current_tol) {
            return { x, fx, iter, Status::Success };
        }

        if (std::isnan(fx) || std::isinf(fx)) return { x, fx, iter, Status::DivergedToNaN };

        if (fa * fx <= 0.0) {
            b = x; fb = fx;
        }
        else {
            a = x; fa = fx;
        }
        x = zero_reta();
        fx = f(x);
    }

    return { x, fx, max_iter, Status::MaxIterationsReached };
}

template <typename Func>
requires std::invocable<Func, std::complex<double>>
auto newton_raphson(Func&& f, double x0,
                double epsilon = std::numeric_limits<double>::epsilon(),
                int32_t max_iter = 100) -> Result
{
    constexpr double h = 1e-100;
    double
        tol_abs {epsilon},
        tol_rel {epsilon};

    for (int32_t iter {0}; iter < max_iter; ++iter) {
        std::complex<double> z = f(std::complex<double>(x0, h));
        double fx = z.real();
        double dfx = z.imag() / h;

        if (std::abs(fx) <= tol_abs) {
            return { x0, fx, iter, Status::Success };
        }

        if (std::abs(dfx) < std::numeric_limits<double>::epsilon()) {
            return { x0, fx, iter, Status::DivergedToNaN };
        }

        double delta = fx / dfx;
        double x1 = x0 - delta;
        double current_tol = tol_abs + tol_rel * std::abs(x0);

        if (std::abs(delta) < current_tol) {
           return { x1, f(std::complex<double>(x1, 0.0)).real(), iter + 1, Status::Success };
        }

        x0 = x1;
        if (std::isnan(x0) || std::isinf(x0)) return { x0, fx, iter, Status::DivergedToNaN };
    }

    return { x0, f(std::complex<double>(x0, 0)).real(), max_iter, Status::MaxIterationsReached };
}

template <typename Func>
requires std::invocable<Func, double>
auto secantes(Func&& f, double x0, double x1,
                double epsilon = std::numeric_limits<double>::epsilon(),
                int32_t max_iter = 1000) -> Result
{
    double
        tol_abs {epsilon},
        tol_rel {epsilon},
        fx0 {f(x0)},
        fx1 {f(x1)};

    for (int32_t iter = 0; iter < max_iter; iter++) {
        if (std::abs(fx1) <= tol_abs) return { x1, fx1, iter, Status::Success };

        if (std::abs(fx1 - fx0) < std::numeric_limits<double>::epsilon()) {
            return { x1, fx1, iter, Status::DivergedToNaN };
        }

        double step = fx1 * (x1 - x0) / (fx1 - fx0);
        double x2 = x1 - step;
        double current_tol = tol_abs + tol_rel * std::abs(x1);

        if (std::abs(step) < current_tol) return { x2, f(x2), iter+1, Status::Success };

        x0 = x1; fx0 = fx1;
        x1 = x2; fx1 = f(x2);

        if (std::isnan(x1) || std::isinf(x1)) return { x1, fx1, iter, Status::DivergedToNaN };
    }

    return { x1, f(x1), max_iter, Status::MaxIterationsReached };
}

int main() {
    auto f = []<typename T>(T x) { return std::pow(x, 3) + x - 10.0; };
    constexpr bool user_input{true};

    double
        x0 {0.0},
        x1{3.0},
        epsilon{std::numeric_limits<double>::epsilon()};

    int32_t MAX_ITER{1'000'000};

    if constexpr (user_input) {
        std::print("Insira dois pontos: ");
        std::cin >> x0 >> x1;
        while (std::abs(x1 - x0) < epsilon) {
            std::print(std::cerr, "Os pontos {} e {} são iguais, escolha outros pontos: ", x0, x1);
            std::cin >> x0 >> x1;
        }

        std::print("Insira o valor da tolerância: ");
        std::cin >> epsilon;

        std::print("Insira o máximo de iterações que o método deve fazer: ");
        std::cin >> MAX_ITER;
        while (MAX_ITER <= 0) {
            std::print(std::cerr, "Números menores ou iguais a 0, ou maiores que {} não são válidos. Insira um número de iterações válido: ", std::numeric_limits<int32_t>::max());
        }
    }

    auto results = {
        bissessao(f, x0, x1, epsilon, MAX_ITER),
        falsa_posicao(f, x0, x1, epsilon, MAX_ITER),
        newton_raphson(f, x0, epsilon, MAX_ITER),
        secantes(f, x0, x1, epsilon, MAX_ITER)
    };

    for (auto result : results) {
        switch (result.status) {
            case Status::Success:
                std::print("O método convergiu!\n");
                std::print("Raiz encontrada: {}\n", result.root);
                std::print("f(raiz)        : {}\n", result.f_root);
                std::print("Iterações      : {}\n", result.iterations);
                break;
            case Status::InvalidInterval:
                std::print(std::cerr, "Erro: Intervalo inicial inválido (f(a) * f(b) > 0).\n");
                break;
            case Status::DivergedToNaN:
                std::print(std::cerr, "Erro: O método divergiu para NaN.\n");
                break;
            case Status::MaxIterationsReached:
                std::print("Número máximo de iterações atingido. Últimos valores computados:\n");
                std::print("Valor da função: {}    x: {}\n", result.f_root, result.root);
                break;
        }
        std::cout << std::endl;
    }
}
