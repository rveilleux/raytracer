#pragma once

#include "Tuple.h"
#include "Matrix.h"

class Ray {
public:
	Ray(const Ray& ray) = default;
	Ray& operator=(const Ray& ray) = default;
	Ray(const Point& p, const Vector& v)
		: origin(p)
		, direction(v)
	{}

	Point Position(double t) const {
		return origin + direction * t;
	}

	Ray Transform(const Matrix<4, 4>& m) const {
		return Ray(m * origin, m * direction);
	}

	Point origin;
	Vector direction;
};
