#pragma once
#include "Shape.h"

class Plane: public Shape
{
public:
	virtual ~Plane() {}
	void LocalIntersect(const Ray& localRay, Intersections& outIntersections) const override;
	std::optional<Intersection> LocalFirstIntersect(const Ray& localRay) const override;
	Vector LocalNormalAt(const Point& localp) const override;
};
