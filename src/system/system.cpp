#include "system.h"
#include <util/parser.h>
#include <util/transformcache.h>
namespace orion {

	void System::render()
	{
		_pre();
		auto film = camera->getFilm();
		int width = film->getWidth(), height = film->getHeight();
		for (int j = 0; j < height; ++j) {
			for (int i = 0; i < width; ++i) {
				Ray ray = camera->generateRay(i, j);

				Spectrum s = integrator->Li(ray, scene);

				film->setSpectrum(i, j, s);
			}

			static uint64_t timecount = 1000;
			if ((int64_t)(Timer::inst()->getElaspedTime() - timecount) > 0) {
				timecount += 1000;
				std::cout << (int)(j / (Float)height * 100) << "%" << std::endl;
			}
		}
	}

	void System::setUp()
	{
		auto parser = Parser::inst();
		parser->makeRenderOption();
		auto &option = parser->getRenderOption();
		scene = std::shared_ptr<Scene>(new Scene(option->accel, option->lights));
		integrator = option->integrator;
		camera = option->camera;
	}

	void System::_init()
	{
		TexManager::init();
		//LogManager::init();
		Timer::init();
		MeshManager::init();
		Parser::init("D:/cpp/orion/xml/bunny.ori");
		TransformCache::init();
	}

	void System::_pre()
	{
		CHECK_INFO(camera != nullptr, "No camera in _System_!");
		CHECK_INFO(integrator != nullptr, "No integrator in _System_!");
		Timer::inst()->reset();
	}
	void System::_post()
	{
		std::cout << Timer::inst()->getElaspedTime() << std::endl;
	}
}
