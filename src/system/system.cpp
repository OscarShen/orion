#include "system.h"
namespace orion {

	void System::render()
	{
		CHECK_INFO(camera != nullptr, "No camera in system!");

		int numTri = 1, numVer = 3;
		int vertexIndices[3]{ 0,1,2 };
		Point3f P[3]{ Point3f(2,0,2), Point3f(0,0,2), Point3f(0,1,2) };

		auto meshdata = MeshManager::inst()->loadMesh("res/cube.obj");
		std::vector<std::shared_ptr<Shape>> mesh = createTriangleMesh(meshdata);


		// just for test
		auto film = camera->getFilm();
		int width = film->getWidth(), height = film->getHeight();
		for (int j = 0; j < height; ++j) {
			for (int i = 0; i < width; ++i) {
				Ray ray = camera->generateRay(i, j);
				for (size_t k = 0; k < mesh.size(); ++k) {
					if (mesh[k]->intersect(ray)) {
						film->setSpectrum(i, j, Spectrum(0.0f));
						break;
					}
					else
						film->setSpectrum(i, j, Spectrum(1.0f));
				}
			}
		}
	}

	void System::_init()
	{
		camera = nullptr;

		TexManager::init();
		//LogManager::init();
		Timer::init();
		MeshManager::init();
	}
}
