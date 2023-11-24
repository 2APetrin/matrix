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
}
