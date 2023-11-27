#include <iostream>
#include <ctime>
#include "matrix.hpp"

int main() {
    int sz = 0;
    std::cin >> sz;
    matrix::matrix_t<double> mtx = matrix::matrix_t<double>::eye(sz);


    int val = 0;
    double vald = 1.0;
    double det = 1.0;
    for (int i = 0; i < sz; ++i) {
        while ((val = (std::rand() % 1000)) == 0);
        vald = static_cast<double>(val) / 350.0;
        mtx[i][i] = vald;
        det *= vald;
    }

    std::cout << "det=" << det << std::endl;

    std::srand(std::time(nullptr));

    int fst = 0;
    int snd = 0;
    for (int i = 0, end = sz*10; i < end; ++i) {
        fst = (std::rand() % sz);
        snd = (std::rand() % sz);
        while (fst == snd) snd = (std::rand() % sz);

        matrix::detail::add_rows(mtx, fst, snd, 1.0);
    }

    for (int i = 0, end = 100; i < end; ++i) {
        fst = (std::rand() % sz);
        snd = (std::rand() % sz);
        while (fst == snd) snd = (std::rand() % sz);

        mtx.swap_rows(fst, snd);
    }

    std::cout << sz << std::endl;
    mtx.dump();
    std::cout << mtx.determinant();
}