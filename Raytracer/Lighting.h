#pragma once
#include "Material.h"
#include "PointLight.h"
#include "Tuple.h"

class Shape;

Color Lighting(const Material& material, const PointLight& light,
 const Point& point, const Vector& eyev, const Vector& normalv, bool inshadow);

Color Lighting(const Material& material, const Shape* object, const PointLight& light,
	const Point& point, const Vector& eyev, const Vector& normalv, bool inshadow);

