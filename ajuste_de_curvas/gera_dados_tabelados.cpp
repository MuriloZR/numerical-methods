#include <iostream>
#include <fstream>
#include <cmath>

// =========================================
// Gera um arquivo com n pontos
// dentro do intervalo [a, b] no formato
//      x   f(x)
// =========================================

int main() {
    int a, b, n;
    std::cin >> a >> b >> n;

    auto f = []<typename T>(T x) { return std::sin(x); };

    std::string s = std::format("tabela-{}entradas", n);
    std::ofstream outFile(s);
    if (outFile.is_open()) {
        outFile << n << '\n';
        double h {static_cast<double>(b-a)/(n-1)};
        for (int i = 0; i < n; i++) {
            double x {static_cast<double>(a) + i*h};
            outFile << x << ' ' << f(x) << '\n';
        }
        outFile.close();
    }
}