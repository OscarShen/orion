#include "system.h"
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
			if(Timer::inst()->getElaspedTime() % 1000 == 0)
				std::cout << (int)(j / (Float)height * 100) << "%" << std::endl;
		}
	}

	bool System::loadScene(const std::string & name)
	{
		return scene.loadScene(name);
	}

	void System::_init()
	{
		TexManager::init();
		//LogManager::init();
		Timer::init();
		MeshManager::init();

		// camera
		std::shared_ptr<PerspectiveCamera> camera(new PerspectiveCamera());
		camera->setOrig(Point3f(25.0f));
		camera->setUp(Vector3f(0.0f, 1.0f, 0.0f));
		camera->setLookat(Point3f(0.0f, 0.0f, 0.0f));
		camera->setFov(45.0f);
		std::shared_ptr<RenderTarget> film(new RenderTarget(1960, 1080));
		camera->setRenderTarget(film);
		this->camera = camera;

		// integrator
		integrator.reset(new WhittedIntegrator());
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
