#pragma once
#include "Shape.h"
#include "PointLight.h"

class World {
public:
	World();
	static World Default();

	int GetObjectCount() const { return _objects.size(); }
	PointLight* GetLightSource() const { return _light.get(); }
	void SetLightSource(std::unique_ptr<PointLight>&& light) { _light = std::move(light); }
	void AddObject(std::unique_ptr<Shape>&& object);
	Shape* GetObject(int index) const { return _objects[index].get(); }
	Intersections Intersect(const Ray& ray) const;
	std::optional<Intersection> GetIntersectHit(const Ray& ray) const;
	Color ShadeHit(const TComputations& comps) const;
	Color ColorAt(const Ray& ray) const;
	bool IsShadowed(const Point& point) const;
	
	MaterialManager _materialManager;
private:
	std::unique_ptr<PointLight> _light;
	std::vector<std::unique_ptr<Shape>> _objects;
};
