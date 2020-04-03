#pragma once
#include "Shape.h"

class Plane: public Shape
{
public:
	void LocalIntersect(const Ray& localRay, Intersections& outIntersections) const override;
	std::optional<Intersection> LocalFirstIntersect(const Ray& localRay) const noexcept override;
	Vector LocalNormalAt(const Point& localp) const noexcept override;
};
