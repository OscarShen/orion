#include "imagetexture.h"
#include <stb_image_aug.h>
namespace orion {
	Spectrum ImageTexture::sample(int x, int y) const
	{
		CHECK_INFO(memory != nullptr && memory->data != nullptr, "no data for texture!");
		_coordFilter(x, y);

		int offset = y * width + x;
		return memory->data[offset];
	}

	// inverse of 255, convenient to convert between float and char
	constexpr Float inv255 = 0.0039215686274509803921568627451f;

	// char to float
	inline Float color_CharToFloat(unsigned char c) {
		return c * inv255;
	}
	unsigned char * loadImage(const std::string & filename, int & width, int & height, int & channels)
	{
		return stbi_load(filename.c_str(), &width, &height, &channels, 0);
	}

	void freeImage(unsigned char *data) {
		stbi_image_free(data);
	}
	std::shared_ptr<ImageMemory> loadImage(const std::string &filename)
	{
		CHECK_INFO(!filename.empty(), "file path is empty");
		int width, height, channel;
		unsigned char * data = loadImage(filename, width, height, channel);
		CHECK_INFO(width != 0 && height != 0, "load a empty image!");
		Spectrum *s = new Spectrum[width * height];
		int offset = 0, cur = 0;
		for (int j = 0; j < height; ++j) {
			for (int i = 0; i < width; ++i) {
				Float r = color_CharToFloat(data[offset++]);
				Float g = color_CharToFloat(data[offset++]);
				Float b = color_CharToFloat(data[offset++]);
				s[cur++] = Spectrum(r, g, b);
			}
		}
		freeImage(data);
		return std::shared_ptr<ImageMemory>(new ImageMemory(s, width, height));;
	}
}
