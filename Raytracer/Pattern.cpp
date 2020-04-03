#include "Pattern.h"
#include "Shape.h"

Color Pattern::PatternAtShape(const Shape* shape, const Point& p) const {
	const Point localPoint = shape->GetInverseTransform() * p;
	const Point patternPoint = GetInverseTransform() * localPoint;
	return PatternAt(patternPoint);
}
