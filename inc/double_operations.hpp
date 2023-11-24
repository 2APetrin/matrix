#pragma once

#include <cmath>

namespace doperations {

const double DOUBLE_ACCURACY = 10e-7;

inline bool is_equal(double num1, double num2) { return (std::abs(num1 - num2) < DOUBLE_ACCURACY); }

}
