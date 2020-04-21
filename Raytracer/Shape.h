#pragma once
#include "Material.h"
#include "Intersections.h"
#include "Ray.h"
#include "BaseTransform.h"

class Shape: public BaseTransform {
public:
	void SetMaterial(Material* m) noexcept { _material = m; }
	Material* GetMaterial() const noexcept { return _material; }
	friend bool operator==(const Shape& lhs, const Shape& rhs) noexcept;
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
	struct N {
		double n1;
		double n2;
	};

	TComputations(const Intersection& intersection, const Ray& ray, const Intersections& intersections);
	const double t;
	const Shape* const object;
	const Point point;
	const Vector eyev;
	const Vector originalNormalv;
	const bool inside;
	const Vector normalv;
	const Point overPoint;
	const Vector reflectv;
	const N n;

private:
	N ComputeSomething(const Intersections& intersections) const;
};

TComputations PrepareComputations(const Intersection& intersection, const Ray& ray);
TComputations PrepareComputations(const Intersection& intersection, const Ray& ray, const Intersections& intersections);
