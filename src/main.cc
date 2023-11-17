#include "matrix.hpp"
#include <iterator>

using namespace matrix;

int main() {
    int sz = 0;
    std::cin >> sz;

    /* matrix_t<int> mtrx{sz, sz, 0};
    mtrx[2][2] = 9;
    mtrx.dump(); */

    matrix_t<int> mtrx2 = matrix_t<int>::eye(sz);
    mtrx2.dump();

    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9};

    matrix_t<int> mtrx3{3, 3, v.begin(), v.end()};
    mtrx3.dump();
}