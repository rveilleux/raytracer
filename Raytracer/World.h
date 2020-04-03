#pragma once
#include "Shape.h"
#include "PointLight.h"

class World {
public:
	World() noexcept;
	static World Default();

	int GetObjectCount() const noexcept { return _objects.size(); }
	PointLight* GetLightSource() const noexcept { return _light.get(); }
	void SetLightSource(std::unique_ptr<PointLight>&& light) noexcept { _light = std::move(light); }
	Shape* AddObject(std::unique_ptr<Shape>&& object);
	Shape* GetObject(int index) const noexcept { return _objects[index].get(); }
	Intersections Intersect(const Ray& ray) const;
	std::optional<Intersection> GetIntersectHit(const Ray& ray) const;
	Color ShadeHit(const TComputations& comps, int remainingRecursion = 1) const;
	Color ColorAt(const Ray& ray, int remainingRecursion = 1) const;
	bool IsShadowed(const Point& point) const;
	Color ReflectedColor(const TComputations& comps, int remainingRecursion = 1) const;
	
	Material* CreateMaterial() { return _materialManager.CreateMaterial(); }
	Material* GetDefaultMaterial() const noexcept { return _materialManager.GetDefaultMaterial(); }

private:
	std::unique_ptr<PointLight> _light;
	std::vector<std::unique_ptr<Shape>> _objects;
	MaterialManager _materialManager;
};
