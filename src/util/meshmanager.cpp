#include "meshmanager.h"

namespace orion {

	DEFINE_SINGLETON(MeshManager);

	void MeshManager::clear()
	{
		for (auto it = models.begin(); it != models.end();) {
			it = models.erase(it);
		}
	}
	std::shared_ptr<ModelData> MeshManager::loadMeshData(const std::string & name)
	{
		auto it = models.find(name);
		if (it != models.end()) {
			return it->second;
		}
		std::shared_ptr<ModelData> modeldata = std::make_shared<ModelData>();
		ModelLoader::loadModel(name, *modeldata);
		CHECK_INFO((*modeldata)[0].num_triangles > 0, "No triangle in mesh!");
		models.insert(std::pair<std::string, std::shared_ptr<ModelData>>(name, modeldata));
		return modeldata;
	}
}
