#include "materialmanager.h"

namespace orion {
	DEFINE_SINGLETON(MaterialManager);

	void MaterialManager::addMaterial(const std::string & name, const std::shared_ptr<Material>& material)
	{
		CHECK_INFO(materials.find(name) == materials.end(), "Already has same material!");
		materials[name] = material;
	}
	std::shared_ptr<Material> MaterialManager::getMaterial(const std::string & name)
	{
		CHECK_INFO(materials.find(name) != materials.end(), "Do not have material : " + name );
		return materials[name];
	}
}
