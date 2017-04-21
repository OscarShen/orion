#include "uvtexture.h"

namespace orion {

	Spectrum UVTexture::sample(int x, int y) const
	{
		_coordFilter(x, y);
		Float u = (Float)x / width;
		Float v = (Float)y / height;
		return Spectrum(u, v, 0.0f);
	}

}
