#include <core/geometry.h>
#include <math/linalg.h>
#include <texture/texture.h>
#include <texture/grid.h>
using namespace orion;

void main()
{
	GridTexture t(Spectrum(0.0f, 0.0f, 1.0f), Spectrum());
	for (int i = 200; i < 800; i += rand() % 100) {
		t.setSize(i, i);
		t.output("res/" + std::to_string(i) + ".bmp");
	}
}

