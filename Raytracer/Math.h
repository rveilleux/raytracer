#pragma once

#include <cmath>
#include <limits>

// Semi-global way to disable 'code analysis warnings' that cannot be disabled thru the /wXXXXX compiler option
// warning C26446: Prefer to use gsl::at() instead of unchecked subscript operator (bounds.4).
#pragma warning (disable: 26446)

inline constexpr bool IsOdd(int number) noexcept { return (number & 1) == 1; }

//RV: std::numeric_limits<double>::epsilon() is too small for some test to succeed
//because epsilon() only works for values at 1.0.
//const double myEpsilon = std::numeric_limits<double>::epsilon() * 100;
constexpr double myEpsilon = 0.000001;
inline bool almostEqual(double a, double b, double delta = myEpsilon) noexcept {
	return std::abs(a - b) <= delta;
}
