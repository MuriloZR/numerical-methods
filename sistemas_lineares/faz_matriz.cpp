#include <iostream>
#include <fstream>
#include <string>
#include <format>

// Faz uma matriz de tamanho NxN com solução x = (1, ..., N)

int main() {
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
    }
}
