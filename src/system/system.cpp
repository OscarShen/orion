#include "system.h"

namespace orion {

	void System::render()
	{
		CHECK_INFO(camera != nullptr, "No camera in system!");

		// just for test
		Bounds3f box(Point3f(0.0f), Point3f(0.2f, 0.1f, 0.3f));
		auto film = camera->getFilm();
		int width = film->getWidth(), height = film->getHeight();
		for (int j = 0; j < height; ++j) {
			for (int i = 0; i < width; ++i) {
				Ray ray = camera->generateRay(i, j);
				if (box.intersect(ray))
					film->setSpectrum(i, j, Spectrum(0.0f));
				else
					film->setSpectrum(i, j, Spectrum(1.0f));
			}
		}
	}

	void System::_init()
	{
		camera = nullptr;

		TexManager::init();
		//LogManager::init();
		Timer::init();
	}
}
