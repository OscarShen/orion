#include "system.h"
#include <util/parser.h>
#include <util/transformcache.h>
namespace orion {

	void System::render()
	{
		_pre();
		auto film = camera->getFilm();
		int width = film->getWidth(), height = film->getHeight();
		integrator->preprocess(*scene, *sampler);
		for (int j = 0; j < height; ++j) {
			for (int i = 0; i < width; ++i) {
				std::vector<Spectrum> ret(nSamples);

				for (int k = 0; k < nSamples; ++k) {
					Ray ray = camera->generateRay(Point2f((Float)i, (Float)j), sampler);

					ret[k] = integrator->Li(ray, scene, sampler, 0);
				}

				Spectrum s(0);
				for (int k = 0; k < nSamples; ++k) {
					s += ret[k];
				}
				s /= (Float)nSamples;
				film->setSpectrum(i, j, s);
			}

			static uint64_t timecount = 5000;
			if ((int64_t)(Timer::inst()->getElaspedTime() - timecount) > 0) {
				timecount += 5000;
				std::cout << (int)(j / (Float)height * 100) << "%" << std::endl;
			}
		}
	}

	void System::setUp()
	{
		auto parser = Parser::inst();
		parser->makeRenderOption();
		auto &option = parser->getRenderOption();
		scene = option->scene;
		integrator = option->integrator;
		camera = option->camera;
		sampler = option->sampler;
		nSamples = option->nSamples;
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
