#include "TestPattern.h"

Color TestPattern::PatternAt(const Point& p) const noexcept {
	return Color(p.x, p.y, p.z);
}
