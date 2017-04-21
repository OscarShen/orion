#include <core/geometry.h>
#include <math/linalg.h>
#include <texture/texture.h>
#include <util/texmanager.h>
#include <util/logmanager.h>
#include <texture/grid.h>
#include <texture/rendertarget.h>
using namespace orion;

void init() {
	TexManager::init();
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
}
