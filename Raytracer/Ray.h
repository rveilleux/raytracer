#pragma once

#include "Tuple.h"
#include "Matrix.h"

class Ray {
public:
	//Ray(const Ray& ray) noexcept = default;
	//Ray& operator=(const Ray& ray) noexcept = default;
	Ray(const Point& p, const Vector& v) noexcept
		: origin(p)
		, direction(v)
	{}

	Point Position(double t) const noexcept {
		return origin + direction * t;
	}

	Ray Transform(const Matrix<4, 4>& m) const noexcept {
		return Ray(m * origin, m * direction);
	}

	Point origin;
	Vector direction;
};
