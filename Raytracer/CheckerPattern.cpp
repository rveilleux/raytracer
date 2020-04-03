#include "CheckerPattern.h"

CheckerPattern::CheckerPattern(const Color& c1, const Color& c2)
	: a(c1)
	, b(c2)
{
}

Color CheckerPattern::PatternAt(const Point& p) const noexcept {
	const double t = (std::abs(std::floor(p.x)) +
		std::abs(std::floor(p.y)) +
		std::abs(std::floor(p.z)));
	if (std::fmod(t, 2) == 0) {
		return a;
	}
	else {
		return b;
	}
}
