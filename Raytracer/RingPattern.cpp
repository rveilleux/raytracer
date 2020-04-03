#include "RingPattern.h"

RingPattern::RingPattern(const Color& c1, const Color& c2) noexcept
	: a(c1)
	, b(c2)
{
}

Color RingPattern::PatternAt(const Point& p) const noexcept {
	const double distance = std::sqrt(p.x * p.x + p.z * p.z);
	// Attempt at making fmod faster. Uncertain.
	//double t = distance - 2 * std::round(distance / 2);
	const double t = std::fmod(distance, 2);
	if (t < 1) {
		return a;
	}
	else {
		return b;
	}
}
