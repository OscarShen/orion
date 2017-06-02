#include "system.h"
#include <util/parser.h>
#include <util/transformcache.h>
#include <util/materialmanager.h>
#include <common/parallel.h>
#include <omp.h>
#include <sampler/sampler.h>
namespace orion {

	void System::render()
	{
		_pre();
		integrator->preprocess(*scene, *sampler);
		std::cout << " [Detected " << numSystemCores() <<" cores]" << std::endl;
		//if (multiThreadEnabled()) {
		if(false){
			_traceMT();
		}
		else {
			std::cout << "trace" << std::endl;
			_trace();
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
		Parser::init("xml/cornell.ori"); // TODO : parse in xml
		TransformCache::init();
		MaterialManager::init();
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
	void System::_trace()
	{
		auto film = camera->getFilm();
		filmWidth = film->getWidth(), filmHeight = film->getHeight();
		pixelFinished = 0;
		for (int j = 0; j < filmHeight; ++j) {
			for (int i = 0; i < filmWidth; ++i) {
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


				++pixelFinished;
				_outputProgress();
			}
		}
	}
	void System::_traceMT()
	{
		auto film = camera->getFilm();
		filmWidth = film->getWidth(), filmHeight = film->getHeight();
		pixelFinished = 0;
#pragma omp parallel for 
		for (int j = 0; j < filmHeight; ++j) {
			for (int i = 0; i < filmWidth; ++i) {
				std::vector<Spectrum> ret(nSamples);

				std::shared_ptr<Sampler> sc = sampler->clone(i * 7 + j * 13);
				for (int k = 0; k < nSamples; ++k) {
					Ray ray = camera->generateRay(Point2f((Float)i, (Float)j), sc);

					ret[k] = integrator->Li(ray, scene, sc, 0);
				}

				Spectrum s(0);
				for (int k = 0; k < nSamples; ++k) {
					s += ret[k];
				}
				s /= (Float)nSamples;
				film->setSpectrum(i, j, s);
#pragma omp critical
				{
					++pixelFinished;
					_outputProgress();
				}
			}
		}
	}
	void System::_outputProgress()
	{
		uint64_t workDone = pixelFinished, totalWork = filmWidth * filmHeight;
		float spendTime = Timer::inst()->getElaspedTime() / 1000.0f;
		static float lastSpendTime = spendTime, intervalTime = -1.0f;
		if (spendTime - lastSpendTime > intervalTime) {
			lastSpendTime = spendTime;
			if (spendTime < 3)
				intervalTime = 0.0f;
			else if (spendTime < 10)
				intervalTime = 0.2f;
			else if (spendTime < 20)
				intervalTime = 0.4f;
			else if (spendTime < 30)
				intervalTime = 0.8f;
			else
				intervalTime = 5.0f;

			float needTime = spendTime * totalWork / workDone - spendTime;
			// output progress
			int progress = (int)((float)(pixelFinished * 50) / (filmWidth * filmHeight));

			int plussesPrinted = 0;

			// Initialize progress string
			const int bufLen = 128;
			std::unique_ptr<char[]> buf(new char[bufLen]);
			snprintf(buf.get(), bufLen, "\r%s: [", "Rendering");
			char *curSpace = buf.get() + strlen(buf.get());
			char *s = curSpace;
			for (int i = 0; i < 50; ++i)
				*s++ = ' ';
			*s++ = ']';
			*s++ = ' ';

			Float percentDone = Float(workDone) / Float(totalWork);
			int plussesNeeded = (int)std::round(50 * percentDone);
			while (plussesPrinted < plussesNeeded) {
				*curSpace++ = '+';
				++plussesPrinted;
			}

			snprintf(s, 20, " (%.1fs|%.1fs)  ", spendTime, needTime);
			fputs(buf.get(), stdout);
			fflush(stdout);
		}
	}
}
