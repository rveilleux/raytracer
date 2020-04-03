#include "Plane.h"

void Plane::LocalIntersect(const Ray& localRay, Intersections& outIntersections) const {
	const auto& optIntersection = LocalFirstIntersect(localRay);
	if (optIntersection) {
		outIntersections.Add(optIntersection.value());
	}
}

std::optional<Intersection> Plane::LocalFirstIntersect(const Ray& localRay) const noexcept {
	if (std::abs(localRay.direction.y) >= myEpsilon) {
		const double t = -localRay.origin.y / localRay.direction.y;
		return Intersection(t, this);
	}
	return std::nullopt;
}

Vector Plane::LocalNormalAt(const Point&) const noexcept {
	return Vector(0, 1, 0);
}
