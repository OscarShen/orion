#include <core/geometry.h>
#include <math/linalg.h>
#include <texture/texture.h>
#include <util/texmanager.h>
#include <util/logmanager.h>
#include <texture/grid.h>
using namespace orion;

void init() {
	TexManager::init();
}

void main()
{
	init();
	std::shared_ptr<GridTexture> g(new GridTexture(Spectrum(0.0f, 0.0, 1.0f), Spectrum()));
	TexManager::inst()->write(g, "aaa.bmp");
	auto image = TexManager::inst()->read("D:/cpp/orion/aaa.bmp");
	TexManager::inst()->write(image, "bbb.bmp");
}
