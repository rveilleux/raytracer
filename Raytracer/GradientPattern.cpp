#include "GradientPattern.h"

GradientPattern::GradientPattern(const Color& c1, const Color& c2)
	: a(c1)
	, b(c2)
{
}

Color GradientPattern::PatternAt(const Point& p) const {
	Color distance = b - a;
	double fraction = p.x - floor(p.x);
	return a + distance * fraction;
}
