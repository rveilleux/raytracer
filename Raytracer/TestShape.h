#pragma once
#include "Shape.h"
#include <optional>

class TestShape: public Shape {
public:
	virtual ~TestShape() {};
	Vector LocalNormalAt(const Point& localp) const override;
	void LocalIntersect(const Ray& localRay, Intersections& outIntersections) const override;
	std::optional<Intersection> LocalFirstIntersect(const Ray& localRay) const override;

	mutable std::optional<Ray> savedRay;
};
