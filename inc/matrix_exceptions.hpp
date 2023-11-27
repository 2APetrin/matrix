#pragma once

#include <stdexcept>

namespace matrix_exceptions {
    struct matrix_bad_size : public std::runtime_error {
        matrix_bad_size() : std::runtime_error("Rows or cols number can't be zero or less\n") {}
    };

    struct bad_elements_cnt : public std::runtime_error {
        bad_elements_cnt() : std::runtime_error("Size of initial array doesn't fit with matrix size\n") {}
    };

    struct matrix_bad_shape : public std::runtime_error {
        matrix_bad_shape() : std::runtime_error("Matrix is not square matrix\n") {}
    };

    struct bad_cols_rows_number : public std::runtime_error {
        bad_cols_rows_number() : std::runtime_error("Given col or row numer is out of bounds\n") {}
    };

    struct bad_matrixes_add : public std::runtime_error {
        bad_matrixes_add() : std::runtime_error("Matrixes are different shapes. You can't add them!\n") {}
    };

    struct bad_matrixes_mul : public std::runtime_error {
        bad_matrixes_mul() : std::runtime_error("Matrixes shapes don't fit. You can't multiply them!\n") {}
    };
}
