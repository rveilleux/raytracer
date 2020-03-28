#include "GlassSphere.h"
#include "Material.h"

Sphere GlassSphere(MaterialManager& materialManager) {
	Sphere s;
	Material* m = materialManager.CreateMaterial();
	s.SetMaterial(m);
	m->transparency = 1;
	m->refractiveIndex = 1.5;
	return s;
}
