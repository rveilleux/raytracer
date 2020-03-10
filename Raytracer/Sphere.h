#pragma once

#include "Shape.h"

class Sphere: public Shape {
public:
	virtual ~Sphere() {};
	void LocalIntersect(const Ray& localRay, Intersections& outIntersections) const override;
	std::optional<Intersection> LocalFirstIntersect(const Ray& localRay) const override;
	Vector LocalNormalAt(const Point& localp) const override;
private:
	std::tuple<std::optional<Intersection>, std::optional<Intersection>> LocalIntersectMultiple(const Ray& localRay) const;
};
