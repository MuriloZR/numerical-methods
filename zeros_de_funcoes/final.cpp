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
               double tol_abs = 1e-15,
               double tol_rel = 1e-15,
               int32_t max_iter = 1000) -> Result
{
    auto pm {[&a, &b](){
        return a+(b-a)/2.0;}
    };

    double fa = f(a);
    double fb = f(b);

    if (fa * fb > 0.0) {
        return { 0.0, 0.0, 0, Status::InvalidInterval };
    }

    double x = pm();
    double fx = f(x);

    for (int32_t iter = 0; iter < max_iter; ++iter) {
        double current_tol = tol_abs + tol_rel * std::max(std::abs(a), std::abs(b));

        if (std::abs(fx) <= tol_abs || std::abs(b - a) < current_tol) {
            return { x, fx, iter, Status::Success };
        }

        if (std::isnan(fx) || std::isinf(fx)) {
            return { x, fx, iter, Status::DivergedToNaN };
        }

        if (fa * fx <= 0.0) {
            b = x; fb = fx;
        }
        else {
            a = x; fa = fx;
        }

        x = pm();
        fx = f(x);
        iter++;
    }

    return { x, fx, max_iter, Status::MaxIterationsReached };
}

template <typename Func>
requires std::invocable<Func, double>
auto falsa_posicao(Func&& f, double a, double b,
               double tol_abs = 1e-15,
               double tol_rel = 1e-15,
               int32_t max_iter = 1000) -> Result
{
    double fa {f(a)};
    double fb {f(b)};
    auto zero_reta {[&a, &fa, &b, &fb](){return a - fa * (b-a)/(fb-fa);}};

    if (fa * fb > 0.0) {
        return { 0.0, 0.0, 0, Status::InvalidInterval };
    }

    double x{zero_reta()};
    double fx = f(x);

    for (int32_t iter = 0; iter < max_iter; ++iter) {
        double current_tol = tol_abs + tol_rel * std::max(std::abs(a), std::abs(b));

        if (std::abs(fx) <= tol_abs || std::abs(b - a) < current_tol) {
            return { x, fx, iter, Status::Success };
        }

        if (std::isnan(fx) || std::isinf(fx)) {
            return { x, fx, iter, Status::DivergedToNaN };
        }

        if (fa * fx <= 0.0) {
            b = x; fb = fx;
        }
        else {
            a = x; fa = fx;
        }

        x = zero_reta();
        fx = f(x);
        iter++;
    }

    return { x, fx, max_iter, Status::MaxIterationsReached };
}

template <typename Func>
requires std::invocable<Func, std::complex<double>>
auto newton_raphson(Func&& f, double x0,
               double tol_abs = 1e-15,
               double tol_rel = 1e-15,
               int32_t max_iter = 100) -> Result
{
    double x = x0;
    constexpr double h = 1e-100; // h extremamente pequeno funciona com passo complexo!

    for (int32_t iter = 0; iter < max_iter; ++iter) {
        std::complex<double> z = f(std::complex<double>(x, h));
        double fx = z.real();
        double dfx = z.imag() / h;

        if (std::abs(fx) <= tol_abs) {
            return { x, fx, iter, Status::Success };
        }

        if (std::abs(dfx) < std::numeric_limits<double>::epsilon()) {
            return { x, fx, iter, Status::DivergedToNaN };
        }

        double delta = fx / dfx;
        double x_next = x - delta;

        double current_tol = tol_abs + tol_rel * std::abs(x);
        if (std::abs(x_next - x) < current_tol) {
            return { x_next, fx, iter, Status::Success };
        }

        x = x_next;
        if (std::isnan(x) || std::isinf(x)) return { x, fx, iter, Status::DivergedToNaN };
    }

    return { x, f(std::complex<double>(x, 0)).real(), max_iter, Status::MaxIterationsReached };
}

template <typename Func>
requires std::invocable<Func, double>
auto secantes(Func&& f, double x0, double x1,
               double tol_abs = 1e-15,
               double tol_rel = 1e-15,
               int32_t max_iter = 1000) -> Result
{
    auto prox_ponto {[&f, &x0, &x1](){
        return x1 - (f(x1)*(x1 - x0))/(f(x1) - f(x0));}
    };

    double x2 {prox_ponto()};

    for (int32_t iter = 0; iter < max_iter; ++iter) {
        double current_tol = tol_abs + tol_rel * std::max(std::abs(x0), std::abs(x1));

        if (std::abs(f(x1)) <= tol_abs || std::abs(x1 - x0) < current_tol) {
            return { x1, f(x1), iter, Status::Success };
        }

        if (std::isnan(x2) || std::isinf(x2)) {
            return { x1, f(x1), iter, Status::DivergedToNaN };
        }

        x0 = x1;
        x1 = x2;
        x2 = prox_ponto();
        iter++;
    }

    return { x2, f(x2), max_iter, Status::MaxIterationsReached };
}

int main() {
    auto f = []<typename T>(T x) { return std::pow(x, 3) + x - 10.0; };
    double x0 {0.0}, x1{3.0};

    auto results = {
        bissessao(f, x0, x1),
        falsa_posicao(f, x0, x1),
        newton_raphson(f, x0),
        secantes(f, x0, x1)
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
