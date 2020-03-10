#include "World.h"
#include "Sphere.h"
#include "PointLight.h"
#include "Lighting.h"

World::World() {
}

void World::AddObject(std::unique_ptr<Shape>&& object) {
	_objects.emplace_back(std::move(object));
}

World World::Default() {
	World w;
	auto light = std::make_unique<PointLight>(Point(-10, 10, -10), Color(1, 1, 1));
	w.SetLightSource(std::move(light));
	auto s1 = std::make_unique<Sphere>();
	auto m1 = w._materialManager.CreateMaterial();
	m1->color = Color(0.8, 1.0, 0.6);
	m1->diffuse = 0.7;
	m1->specular = 0.2;
	s1->SetMaterial(m1);
	auto s2 = std::make_unique<Sphere>();
	s2->SetMaterial(w._materialManager.GetDefaultMaterial());
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

Color World::ShadeHit(const TComputations& comps) const {
	bool shadowed = IsShadowed(comps.overPoint);
	Material* objectMat = comps.object->GetMaterial();
	_ASSERT_EXPR(objectMat, L"Object must have a material to be renderable");
	return Lighting(*objectMat, comps.object,
		*GetLightSource(), comps.point, comps.eyev, comps.normalv, shadowed);
}

Color World::ColorAt(const Ray& ray) const {
	const auto& hit = GetIntersectHit(ray);
	if (!hit) {
		return Color();
	}
	const auto& comps = PrepareComputations(hit.value(), ray);
	return ShadeHit(comps);
}

bool World::IsShadowed(const Point& point) const {
	Point lightp = GetLightSource()->GetPosition();
	Vector light2point = point - lightp;
	double distance = light2point.Magnitude();
	Vector lightdir = light2point.Normalize();
	Ray r = Ray(lightp, lightdir);
	const auto& hit = GetIntersectHit(r);
	if (hit && hit->t < distance) {
		return true;
	}
	else {
		return false;
	}
}