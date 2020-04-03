#pragma once
#include "Color.h"
#include "BaseTransform.h"
class Shape;

class Pattern : public BaseTransform {
public:
	virtual Color PatternAt(const Point& p) const = 0;

	Color PatternAtShape(const Shape* shape, const Point& p) const;
};
