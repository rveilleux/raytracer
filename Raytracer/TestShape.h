#pragma once
#include "Shape.h"
#include <optional>

class TestShape: public Shape {
public:
	Vector LocalNormalAt(const Point& localp) const noexcept override;
	void LocalIntersect(const Ray& localRay, Intersections& outIntersections) const noexcept override;
	std::optional<Intersection> LocalFirstIntersect(const Ray& localRay) const noexcept override;

	mutable std::optional<Ray> savedRay;
};
