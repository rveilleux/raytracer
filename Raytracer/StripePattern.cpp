#include "StripePattern.h"

StripePattern::StripePattern(const Color& c1, const Color& c2)
	: a(c1)
	, b(c2)
{
}

Color StripePattern::PatternAt(const Point& p) const noexcept {
	// Attempt at making fmod faster. Uncertain.
	double t = p.x - 2 * std::round(p.x / 2);
	//double t = std::fmod(p.x, 2);
	if (t < 0) t += 2;
	if (t < 1) {
		return a;
	}
	else {
		return b;
	}
}
