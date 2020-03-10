#include "TestPattern.h"

Color TestPattern::PatternAt(const Point& p) const {
	return Color(p.x, p.y, p.z);
}
