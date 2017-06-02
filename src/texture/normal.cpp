#include "normal.h"

namespace orion {

	Spectrum NormalTexture::sample(int x, int y) const
	{
		CHECK_INFO(false, "Normal texture does not support output.");
		abort();
		return Spectrum();
	}

	Spectrum NormalTexture::evaluate(const Intersection * isec) const
	{
		if (isec)
			return Spectrum(isec->n.x, isec->n.y, isec->n.z);
		return Spectrum();
	}

}
