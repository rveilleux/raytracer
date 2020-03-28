#pragma once
#include "Material.h"
#include "Intersections.h"
#include "Ray.h"
#include "BaseTransform.h"

class Shape: public BaseTransform {
public:
	virtual ~Shape() {};
	void SetMaterial(Material* m) { _material = m; }
	Material* GetMaterial() const { return _material; }
	friend bool operator==(const Shape& lhs, const Shape& rhs);
	Vector NormalAt(const Point& p) const;
	virtual Vector LocalNormalAt(const Point& localp) const = 0;
	Intersections Intersect(const Ray& ray) const;
	void Intersect(const Ray& ray, Intersections& outIntersections) const;
	virtual void LocalIntersect(const Ray& localRay, Intersections& outIntersections) const = 0;
	std::optional<Intersection> FirstIntersect(const Ray& ray) const;
	virtual std::optional<Intersection> LocalFirstIntersect(const Ray& localRay) const = 0;

protected:
	Material* _material = nullptr;// = Material::GetDefaultMaterial();
};

struct TComputations {
	TComputations(const Intersection& intersection, const Ray& ray, const Intersections& intersections)
		: t(intersection.t)
		, object(intersection.object)
		, point(ray.Position(t))
		, eyev(-ray.direction)
		, originalNormalv(object->NormalAt(point))
		, inside(originalNormalv.Dot(eyev) < 0)
		, normalv(inside ? -originalNormalv : originalNormalv)
		, overPoint(point + normalv * myEpsilon)
		, reflectv(ray.direction.Reflect(this->normalv))
		, n1(0)
		, n2(0)
	{
	}
	const double t;
	const Shape* const object;
	const Point point;
	const Vector eyev;
	const Vector originalNormalv;
	const bool inside;
	const Vector normalv;
	const Point overPoint;
	const Vector reflectv;
	const double n1;
	const double n2;
};

TComputations PrepareComputations(const Intersection& intersection, const Ray& ray);
TComputations PrepareComputations(const Intersection& intersection, const Ray& ray, const Intersections& intersections);
