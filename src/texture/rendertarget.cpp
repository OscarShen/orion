#include "rendertarget.h"

namespace orion {
	Spectrum RenderTarget::sample(int x, int y) const
	{
		CHECK_INFO(data.get() != nullptr, "No memory in the render target!");
		_coordFilter(x, y);
		int offset = y * width + x;

		return data[offset];
	}
	void RenderTarget::setSize(int width, int height)
	{
		CHECK_INFO(width > 0 && height > 0, "size of image should larger than 0!")
		this->width = width;
		this->height = height;
		data.reset(new Spectrum[width * height]);
	}

	void RenderTarget::setSpectrum(int x, int y, const Spectrum & s)
	{
		CHECK_INFO(data.get() != nullptr, "No memory in the render target!");
		_coordFilter(x, y);
		int offset = y * width + x;
		data[offset] = s;
	}

}
