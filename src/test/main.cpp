#include <core/geometry.h>
#include <math/linalg.h>
#include <texture/texture.h>
#include <util/texmanager.h>
#include <util/logmanager.h>
#include <texture/checkboard.h>
#include <texture/rendertarget.h>
#include <texture/mixtexture.h>
#include <texture/gridtexture.h>
#include <util/timer.h>
using namespace orion;

void init() {
	TexManager::init();
	Timer::init();
}

void main()
{
	init();
	auto rendertarget = std::make_shared<RenderTarget>(80, 60);
	for (int j = 0; j < 60; ++j) {
		Float a = (Float)j / 60;
		Spectrum s(a, a, a);
		for (int i = 0; i < 80; ++i) {
			rendertarget->setSpectrum(i, j, s);
		}
	}
	TexManager::inst()->write(rendertarget, "aaa.bmp");

	auto floattexture = std::make_shared<FloatTexture>(1, 1, 0.5f);
	auto checktexture = std::make_shared<CheckBoardTexture>(80, 60, Spectrum(0.0f, 0.0f, 1.0f), Spectrum());
	auto mixtexture = std::make_shared<MixTexture>(rendertarget, checktexture, floattexture);
	TexManager::inst()->write(mixtexture, "bbb.bmp");

	auto gridtexture = std::make_shared<GridTexture>(1000, 1000, Spectrum(0.0f, 0.0f, 1.0f), Spectrum(),0.5f);
	TexManager::inst()->write(gridtexture, "ccc.bmp");

	Timer::inst()->reset();
	//Sleep(1000);
	std::cout << Timer::inst()->getElaspedTime() << std::endl;
}
