#include "floattexture.h"

namespace orion {
	FloatTexture::FloatTexture(int width, int height, Float v)
	{
		CHECK_INFO(width > 0 && height > 0, "size of image should larger than 0!");
		setSize(width, height);
		for (int i = 0; i < width * height; ++i) {
			data[i] = v;
		}
	}
	void FloatTexture::setFloat(int x, int y, Float v)
	{
		CHECK_INFO(data.get() != nullptr, "No memory in the float texture!");
		_coordFilter(x, y);
		int offset = y * width + x;
		data[offset] = v;
	}

	Spectrum FloatTexture::sample(int x, int y) const
	{
		CHECK_INFO(data.get() != nullptr, "No memory in the render target!");
		_coordFilter(x, y);
		int offset = y * width + x;

		return Spectrum(data[offset]);
	}

	void FloatTexture::setSize(int width, int height)
	{
		CHECK_INFO(width > 0 && height > 0, "size of image should larger than 0!");
		this->width = width;
		this->height = height;
		data.reset(new Float[width * height]);
	}

}
