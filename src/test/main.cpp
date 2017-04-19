#include <core/geometry.h>
#include <math/linalg.h>
#include <texture/texture.h>
#include <util/texmanager.h>
#include <texture/grid.h>
using namespace orion;

void main()
{
	GridTexture t(Spectrum(0.0f, 0.0f, 1.0f), Spectrum());
	t.setSize(100, 100);
	TexManager::init();
	TexManager::inst()->output(&t, "aaa.bmp");
	TexManager::_delete();
	TexManager::inst()->output(&t, "bbb.bmp");
}
