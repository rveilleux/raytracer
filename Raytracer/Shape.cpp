#include "Shape.h"

void Shape::Intersect(const Ray& ray, Intersections& outIntersections) const {
	const Ray localRay = ray.Transform(GetInverseTransform());
	LocalIntersect(localRay, outIntersections);
}

Intersections Shape::Intersect(const Ray& ray) const {
	Intersections intersections;
	Intersect(ray, intersections);
	return intersections;
}

std::optional<Intersection> Shape::FirstIntersect(const Ray& ray) const {
	const Ray localRay = ray.Transform(GetInverseTransform());
	return LocalFirstIntersect(localRay);
}

Vector Shape::NormalAt(const Point& p) const {
	const Point localPoint = GetInverseTransform() * p;
	const Vector localNormal = LocalNormalAt(localPoint);
	auto worldNormal = GetInverseTransform().Transpose() * localNormal;
	worldNormal.w = 0;
	return worldNormal.Normalize();
}

bool operator==(const Shape& lhs, const Shape& rhs) noexcept {
	return static_cast<const BaseTransform&>(lhs) == static_cast<const BaseTransform&>(rhs) &&
		*lhs.GetMaterial() == *rhs.GetMaterial();
}

TComputations PrepareComputations(const Intersection& intersection, const Ray& ray) {
	Intersections tempIntersections;
	tempIntersections.Add(intersection);
	return PrepareComputations(intersection, ray, tempIntersections);
}

TComputations PrepareComputations(const Intersection& intersection, const Ray& ray, const Intersections& intersections) {
	TComputations comps(intersection, ray, intersections);
	//comps.t = intersection.t;
	//comps.object = intersection.object;
	//comps.point = ray.Position(comps.t);
	//comps.eyev = -ray.direction;
	//comps.normalv = comps.object->NormalAt(comps.point);
	//if (comps.normalv.Dot(comps.eyev) < 0) {
	//	comps.inside = true;
	//	comps.normalv = -comps.normalv;
	//}
	//comps.overPoint = comps.point + comps.normalv * myEpsilon;
	return comps;
}
