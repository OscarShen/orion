#include "system.h"
namespace orion {

	void System::render()
	{
		CHECK_INFO(camera != nullptr, "No camera in system!");
		auto film = camera->getFilm();
		int width = film->getWidth(), height = film->getHeight();
		for (int j = 0; j < height; ++j) {
			for (int i = 0; i < width; ++i) {
				Ray ray = camera->generateRay(i, j);
				Intersection isec;
				if (scene.intersect(ray, &isec)) {
					film->setSpectrum(i, j, Spectrum(std::abs(isec.n.x),
						std::abs(isec.n.y), std::abs(isec.n.z)));
				}
				else {
					film->setSpectrum(i, j, Spectrum(0));
				}
			}
		}
	}

	bool System::loadScene(const std::string & name)
	{
		return scene.loadScene(name);
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
