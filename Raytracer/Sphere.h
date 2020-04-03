#pragma once

#include "Shape.h"

class Sphere: public Shape {
public:
	void LocalIntersect(const Ray& localRay, Intersections& outIntersections) const override;
	std::optional<Intersection> LocalFirstIntersect(const Ray& localRay) const noexcept override;
	Vector LocalNormalAt(const Point& localp) const noexcept override;
private:
	std::tuple<std::optional<Intersection>, std::optional<Intersection>> LocalIntersectMultiple(const Ray& localRay) const noexcept;
};
