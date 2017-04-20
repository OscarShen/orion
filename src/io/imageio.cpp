#include "imageio.h"

namespace orion {
	bool loadImage(const std::string & filename, std::shared_ptr<ImageMemory> &mem)
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
		mem.reset(new ImageMemory(s, width, height));
		return true;
	}
	unsigned char * loadImage(const std::string & filename, int & width, int & height, int & channels)
	{
		return stbi_load(filename.c_str(), &width, &height, &channels, 0);
	}

	void freeImage(unsigned char *data) {
		stbi_image_free(data);
	}
}
