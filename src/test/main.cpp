#include <core/geometry.h>
#include <math/linalg.h>
#include <texture/texture.h>
#include <util/texmanager.h>
#include <util/logmanager.h>
#include <texture/grid.h>
using namespace orion;

void init() {
	//TexManager::init();
	LogManager::init();
}

void main()
{
	init();
	GridTexture t(Spectrum(0.0f, 0.0f, 1.0f), Spectrum());
	t.setSize(100, 100);
	TexManager::inst()->output(&t, "aaa.bmp");
}
