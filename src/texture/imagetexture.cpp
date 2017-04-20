#include "imagetexture.h"

namespace orion {
	Spectrum ImageTexture::sample(int x, int y) const
	{
		CHECK_INFO(memory != nullptr && memory->data != nullptr, "no data for texture!");
		_coordFilter(x, y);

		int offset = y * width + x;
		return memory->data[offset];
	}
}
