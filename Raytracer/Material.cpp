#include "Material.h"
//
//std::shared_ptr<Material> Material::GetDefaultMaterial() {
//	static std::shared_ptr<Material> staticDefaultMaterial = std::make_shared<Material>();
//	return staticDefaultMaterial;
//}

bool operator==(const Material& lhs, const Material& rhs) noexcept {
	return lhs.color == rhs.color &&
		lhs.ambient == rhs.ambient &&
		lhs.diffuse == rhs.diffuse &&
		lhs.shininess == rhs.shininess &&
		lhs.specular == rhs.specular &&
		(((lhs.pattern && rhs.pattern) && (*lhs.pattern == *rhs.pattern)) ||
		(!lhs.pattern && !rhs.pattern));
}

MaterialManager::MaterialManager() {
	_defaultMaterial = CreateMaterial();
}

Material* MaterialManager::CreateMaterial() {
	_materials.emplace_back(Material());
	return &_materials.back();
}
