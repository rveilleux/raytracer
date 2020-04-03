#include "GradientPattern.h"

GradientPattern::GradientPattern(const Color& c1, const Color& c2) noexcept
	: a(c1)
	, b(c2)
{
}

Color GradientPattern::PatternAt(const Point& p) const noexcept {
	const Color distance = b - a;
	const double fraction = p.x - floor(p.x);
	return a + distance * fraction;
}
