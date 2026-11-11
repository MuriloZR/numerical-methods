#pragma once
#include <cstdint>
#include <vector>
#include <print>

template<typename T>
struct Matriz {
    std::uint32_t rows, cols;
    std::vector<T> data;

    explicit Matriz(std::size_t n):
        rows(n), cols(n), data(n * n, T{})
    {}

    Matriz(std::size_t r, std::size_t c):
        rows(r), cols(c), data(r * c, T{})
    {}

    Matriz(std::size_t r, std::size_t c, T val):
        rows(r), cols(c), data(r * c, val)
    {}

    [[nodiscard]] inline T& operator()(std::size_t i, std::size_t j) noexcept {
        return data[i * cols + j];
    }

    [[nodiscard]] inline const T& operator()(std::size_t i, std::size_t j) const noexcept {
        return data[i * cols + j];
    }

    void print() const {
        for (std::size_t i = 0; i < rows; ++i) {
            for (std::size_t j = 0; j < cols; ++j) {
                std::print("{:10.4f} ", (*this)(i, j));
            }
            std::print("\n");
        }
    }
};

template<typename T>
struct tabela {
    std::vector<T> x;
    std::vector<T> fx;

    tabela(size_t n)
        : x(n, 0), fx(n, 0)
    {}

    tabela(const std::vector<T> &x, const std::vector<T> &y)
        : x(x), fx(y)
    {}
};