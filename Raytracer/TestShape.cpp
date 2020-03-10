#include "TestShape.h"

Vector TestShape::LocalNormalAt(const Point& localp) const {
	return Vector(localp.x, localp.y, localp.z);
}

void TestShape::LocalIntersect(const Ray& localRay, Intersections&) const {
	savedRay = localRay;
}

std::optional<Intersection> TestShape::LocalFirstIntersect(const Ray&) const {
	return std::nullopt;
}
