#include "Lighting.h"

Color Lighting(const Material& material, const PointLight& light,
	const Point& point, const Vector& eyev, const Vector& normalv, bool inshadow) {
	return Lighting(material, nullptr, light, point, eyev, normalv, inshadow);
}

Color Lighting(const Material& material, const Shape* object, const PointLight& light,
 const Point& point, const Vector& eyev, const Vector& normalv, bool inshadow) {
	const Color matColor = [&]() {
		if (!material.pattern) return material.color;
		if (!object) return material.pattern->PatternAt(point);
		return material.pattern->PatternAtShape(object, point);
	}();
	const Color effectiveColor = matColor * light.GetIntensity();
	const Vector lightv = (light.GetPosition() - point).Normalize();
	const Color ambient = effectiveColor * material.ambient;
	const double lightDotNormal = lightv.Dot(normalv);
	Color diffuse;
	Color specular;
	// if light is on the other side of the surface OR is in the shadow, 
	// leave diffuse and specular black. Only ambient will be used.
	if (lightDotNormal < 0 || inshadow) {
	}
	else {
		diffuse = effectiveColor * material.diffuse * lightDotNormal;
		const Vector reflectv = (-lightv).Reflect(normalv);
		const double reflectDotEye = reflectv.Dot(eyev);
		if (reflectDotEye <= 0) {

		}
		else {
			const double factor = std::pow(reflectDotEye, material.shininess);
			specular = light.GetIntensity() * material.specular * factor;
		}
	}
	return ambient + diffuse + specular;
}