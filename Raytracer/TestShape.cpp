#include "TestShape.h"

Vector TestShape::LocalNormalAt(const Point& localp) const noexcept {
	return Vector(localp.x, localp.y, localp.z);
}

void TestShape::LocalIntersect(const Ray& localRay, Intersections&) const noexcept {
	savedRay = localRay;
}

std::optional<Intersection> TestShape::LocalFirstIntersect(const Ray&) const noexcept {
	return std::nullopt;
}
