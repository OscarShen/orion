#include "texture.h"

namespace orion {
	void Texture::_coordFilter(int & u, int & v) const
	{
		switch (filter)
		{
		case orion::TextureFilter::FILTER_WRAP:
		{
			if (u >= 0)
				u = u % width;
			else
				u = width - (-u) % width;
			if (v >= 0)
				v = v % height;
			else
				v = height - (-v) % height;

			break;
		}
		case orion::TextureFilter::FILTER_CLAMP:
		{
			u = std::min(width, std::max(u, 0));
			v = std::min(height, std::max(v, 0));
			break;
		}
		case orion::TextureFilter::FILTER_MIRROR:
		{
			u = std::abs(u);
			u %= 2 * width;
			u = u < width ? u : 2 * width - u - 1;
			v = std::abs(v);
			v %= 2 * height;
			v = v < height ? v : 2 * height - v - 1;
			break;
		}
		}
	}
}