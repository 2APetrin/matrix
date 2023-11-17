#include "matrix.hpp"

using namespace matrix;

int main() {
    int sz = 0;
    std::cin >> sz;

    matrix_t<int> mtrx{5, 5, 1};

    mtrx.print();
}