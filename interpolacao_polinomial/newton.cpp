#include <vector>

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

int main() {

}