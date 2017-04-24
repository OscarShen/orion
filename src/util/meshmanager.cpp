#include "meshmanager.h"

namespace orion {

	DEFINE_SINGLETON(MeshManager);

	void MeshManager::clear()
	{
		for (auto it = meshes.begin(); it != meshes.end();) {
			it = meshes.erase(it);
		}
	}
	std::shared_ptr<MeshData> MeshManager::loadMesh(const std::string & name)
	{
		auto it = meshes.find(name);
		if (it != meshes.end()) {
			return it->second;
		}
		std::shared_ptr<MeshData> mesh = std::make_shared<MeshData>();
		ModelLoader::loadModel(name, *mesh.get());
		CHECK_INFO(mesh->num_triangles > 0, "No triangle in mesh!");
		meshes.insert(std::pair<std::string, std::shared_ptr<MeshData>>(name, mesh));
		return mesh;
	}

}
