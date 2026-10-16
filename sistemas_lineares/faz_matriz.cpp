#include <iostream>
#include <fstream>
#include <string>
#include <format>
#include <vector>

// Faz uma matriz de tamanho NxN com solução x = (1, ..., N)

bool sistema_texto() {
    int n;
    std::cin >> n;
    std::string s = std::format("matriz{}x{}", n, n);
    std::ofstream outFile(s);
    if (outFile.is_open()) {
        outFile << n << '\n';
        for (int i = 0; i < n; i++) {
            for (int j = 0; j <= n; j++) {
                if (i == j) outFile << 2;
                else if (j < n) outFile << 1;
                else outFile << n*(1 + n)/2 + i+1;

                if (j < n) outFile << ' ';
                else outFile << '\n';
            }
        }
        outFile.close();
        return true;
    }
    else return false;
}

bool sistema_binario() {
    std::size_t n;
    if (!(std::cin >> n)) return false;

    std::string s = std::format("matriz{}x{}.bin", n, n);
    std::ofstream outFile(s, std::ios::binary);

    if (!outFile) {
        std::cerr << "Erro ao criar o arquivo binario!\n";
        return false;
    }

    // 1. Grava a dimensão N como std::size_t (8 bytes)
    outFile.write(reinterpret_cast<const char*>(&n), sizeof(n));

    // 2. Aloca os vetores contíguos na memória
    std::vector<double> A(n * n);
    std::vector<double> b(n);

    double sum_x = static_cast<double>(n * (1 + n)) / 2.0;

    for (std::size_t i = 0; i < n; i++) {
        for (std::size_t j = 0; j < n; j++) {
            A[i * n + j] = (i == j) ? 2.0 : 1.0;
        }
        b[i] = sum_x + static_cast<double>(i + 1);
    }

    // 3. Escreve a matriz A e o vetor B em dois blocos brutos de bytes
    outFile.write(reinterpret_cast<const char*>(A.data()), A.size() * sizeof(double));
    outFile.write(reinterpret_cast<const char*>(b.data()), b.size() * sizeof(double));

    outFile.close();
    return true;
}

int main() {
    sistema_binario();
}
