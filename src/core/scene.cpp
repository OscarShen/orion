#include "scene.h"

namespace orion {

	bool Scene::loadScene(const std::string & name)
	{
		auto meshdata = MeshManager::inst()->loadMeshData("res/cube.obj");
		auto triVec = createTriangleMesh(meshdata);
		shapes.insert(shapes.end(), triVec.begin(), triVec.end());
		return true;
	}

	bool Scene::intersect(const Ray & ray, Intersection *isec) const
	{
		return Scene::_bruteForce(ray, isec);
	}

	bool Scene::_bruteForce(const Ray & ray, Intersection *isec) const
	{
		bool hit(false);
		Float t = fInfinity;
		int n = (int)shapes.size();
		for (int i = 0; i < n; ++i) {
			Intersection in;
			if (shapes[i]->intersect(ray, &in) && in.t < t) {
				hit = true;
				t = in.t;
				*isec = in;
			}
		}
		return hit;
	}

}
