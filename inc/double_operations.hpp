#pragma once

#include <cmath>

namespace doperations {

const double ACCURACY = 0.00001;

inline bool is_equal(double num1, double num2) { return (std::abs(num1 - num2) < ACCURACY); }

}