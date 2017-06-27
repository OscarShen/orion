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
		int *counters = new int[width * height];
		memset(counters, 0, width * height * sizeof(int));
		counter.reset(counters);
	}

	void RenderTarget::acumulate(int x, int y, const Spectrum & s)
	{
		int offset = y * width + x;
		data[offset] += s;
		++counter[offset];
	}
	std::shared_ptr<RenderTarget> RenderTarget::getAveraged()
	{
		int samples = width * height;
		std::shared_ptr<RenderTarget> r(new RenderTarget(width, height));
		for (int i = 0; i < samples; ++i) {
			r->data[i] = this->data[i] / this->counter[i];
			r->counter[i] = 1;
		}
		return r;
	}
}
