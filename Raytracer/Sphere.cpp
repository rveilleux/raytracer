#include "Sphere.h"

std::tuple<std::optional<Intersection>, std::optional<Intersection>> Sphere::LocalIntersectMultiple(const Ray& localRay) const {
	auto sphere_to_ray = localRay.origin - Point(0, 0, 0);
	auto a = localRay.direction.Dot(localRay.direction);
	auto b = 2 * localRay.direction.Dot(sphere_to_ray);
	auto c = sphere_to_ray.Dot(sphere_to_ray) - 1;
	auto discriminant = b * b - 4 * a * c;
	if (discriminant < 0) return { std::nullopt, std::nullopt };
	double sq = std::sqrt(discriminant);
	auto t1 = (-b - sq) / (2 * a);
	auto t2 = (-b + sq) / (2 * a);
	return { Intersection{ t1, this }, Intersection{ t2, this } };
}

void Sphere::LocalIntersect(const Ray& localRay, Intersections& outIntersections) const {
	const auto& result = LocalIntersectMultiple(localRay);
	if (std::get<0>(result))
	{
		outIntersections.Add(std::get<0>(result).value());
		outIntersections.Add(std::get<1>(result).value());
	}
}

std::optional<Intersection> Sphere::LocalFirstIntersect(const Ray& localRay) const {
	const auto& result = LocalIntersectMultiple(localRay);
	if (std::get<0>(result))
	{
		if (std::get<0>(result)->t < std::get<1>(result)->t) {
			return std::get<0>(result);
		}
		else {
			return std::get<1>(result);
		}
	}
	return std::nullopt;
}

Vector Sphere::LocalNormalAt(const Point& localp) const {
	return localp - Point(0, 0, 0);
}
