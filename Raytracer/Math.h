#pragma once

#include <cmath>
#include <limits>

inline bool IsOdd(int number) { return (number & 1) == 1; }

//RV: std::numeric_limits<double>::epsilon() is too small for some test to succeed
//because epsilon() only works for values at 1.0.
//const double myEpsilon = std::numeric_limits<double>::epsilon() * 100;
const double myEpsilon = 0.000001;
inline bool almostEqual(double a, double b, double delta = myEpsilon) {
	return std::abs(a - b) <= delta;
}
