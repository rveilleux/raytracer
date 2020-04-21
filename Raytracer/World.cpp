#include "World.h"
#include "Sphere.h"
#include "PointLight.h"
#include "Lighting.h"

World::World() noexcept {
}

Shape* World::AddObject(std::unique_ptr<Shape>&& object) {
	const auto& added = _objects.emplace_back(std::move(object));
	return added.get();
}

World World::Default() {
	World w;
	auto light = std::make_unique<PointLight>(Point(-10, 10, -10), Color(1, 1, 1));
	w.SetLightSource(std::move(light));
	auto s1 = std::make_unique<Sphere>();
	auto m1 = w.CreateMaterial();
	m1->color = Color(0.8, 1.0, 0.6);
	m1->diffuse = 0.7;
	m1->specular = 0.2;
	s1->SetMaterial(m1);
	auto s2 = std::make_unique<Sphere>();
	s2->SetMaterial(w.GetDefaultMaterial());
	s2->SetTransform(Scaling({ 0.5,0.5,0.5 }));
	w.AddObject(std::move(s1));
	w.AddObject(std::move(s2));
	return w;
}

Intersections World::Intersect(const Ray& ray) const {
	Intersections xs;
	for (const auto& object : _objects) {
		//auto objIntersections = object->Intersect(ray);
		//if (objIntersections) {
		//	xs.Add(objIntersections.value());
		//}
		object->Intersect(ray, xs);
	}
	return xs;
}

std::optional<Intersection> World::GetIntersectHit(const Ray& ray) const {
	std::optional<Intersection> best;
	for (const auto& object : _objects) {
		auto objIntersection = object->FirstIntersect(ray);
		if (objIntersection && objIntersection->t >= 0) {
			if (!best || best->t > objIntersection->t) {
				best = objIntersection;
			}
		}
	}
	return best;
}

Color World::ShadeHit(const TComputations& comps, int remainingRecursion) const {
	const bool shadowed = IsShadowed(comps.overPoint);
	Material* const objectMat = comps.object->GetMaterial();
	_ASSERT_EXPR(objectMat, L"Object must have a material to be renderable");
	__analysis_assume(objectMat);
	const Color surface = Lighting(*objectMat, comps.object,
		*GetLightSource(), comps.point, comps.eyev, comps.normalv, shadowed);
	const Color reflected = ReflectedColor(comps, remainingRecursion);
	return surface + reflected;
}

Color World::ColorAt(const Ray& ray, int remainingRecursion) const {
	// This was the 'fast version', before handling transparency: Only first hit was considered.
	//const auto& hit = GetIntersectHit(ray);
	//if (!hit) {
	//	return Color();
	//}
	//const auto& comps = PrepareComputations(hit.value(), ray);
	const auto& xs = Intersect(ray);
	const auto& hit = xs.Hit();
	if (!hit) {
		return Color();
	}
	const auto& comps = PrepareComputations(hit.value(), ray, xs);
	return ShadeHit(comps, remainingRecursion);
}

bool World::IsShadowed(const Point& point) const {
	const Point lightp = GetLightSource()->GetPosition();
	const Vector light2point = point - lightp;
	const double distance = light2point.Magnitude();
	const Vector lightdir = light2point.Normalize();
	const Ray r = Ray(lightp, lightdir);
	const auto& hit = GetIntersectHit(r);
	if (hit && hit->t < distance) {
		return true;
	}
	else {
		return false;
	}
}

Color World::ReflectedColor(const TComputations& comps, int remainingRecursion) const {
	if (remainingRecursion <= 0) {
		return Color::Black;
	}
	Material* objectMat = comps.object->GetMaterial();
	_ASSERT_EXPR(objectMat, L"Object must have a material to be reflective");
	if (almostEqual(objectMat->reflective, 0)) {
		return Color::Black;
	}
	const Ray ray = Ray(comps.overPoint, comps.reflectv);
	const Color reflectColor = ColorAt(ray, remainingRecursion - 1);
	return reflectColor * objectMat->reflective;
}
