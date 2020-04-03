#pragma once
#include "Color.h"
#include "Pattern.h"
#include <list>

struct Material {
	Color color = Color(1, 1, 1);
	double ambient = 0.1;
	double diffuse = 0.9;
	double specular = 0.9;
	double shininess = 200.0;
	double reflective = 0;
	double transparency = 0;
	double refractiveIndex = 1;
	std::unique_ptr<Pattern> pattern;
	friend bool operator==(const Material& lhs, const Material& rhs) noexcept;
};

class MaterialManager {
public:
	MaterialManager();
	Material* CreateMaterial();
	Material* GetDefaultMaterial() const noexcept { return _defaultMaterial; }
private:
	std::list<Material> _materials;
	Material* _defaultMaterial;
};
