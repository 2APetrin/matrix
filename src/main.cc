#include "matrix.hpp"
#include <iterator>

using namespace matrix;

int main() {
    int sz = 0;

    std::cin >> sz;
    std::vector<double> v;

    v.reserve(sz*sz);

    double val = 0;
    for (int i = 0, end = sz*sz; i < end; ++i) {
        std::cin >> val;
        v.push_back(val);
    }

    matrix_t<double> mtrx3{sz, sz, v.begin(), v.end()};
    mtrx3.transpose();

    double det = mtrx3.determinant();
    std::cout << "determinant = " << det << std::endl;
}