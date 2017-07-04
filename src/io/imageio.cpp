#include "imageio.h"

namespace orion {
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
	unsigned char * loadImage(const std::string & filename, int & width, int & height, int & channels)
	{
		return stbi_load(filename.c_str(), &width, &height, &channels, 0);
	}

	void freeImage(unsigned char *data) {
		stbi_image_free(data);
	}
	typedef struct
	{
		unsigned long   bfSize;
		unsigned short  bfReserved1;
		unsigned short  bfReserved2;
		unsigned long   bfOffBits;
	} BitMapFileHeader;

	typedef struct
	{
		unsigned long  biSize;
		long           biWidth;
		long           biHeight;
		unsigned short biPlanes;
		unsigned short biBitCount;
		unsigned long  biCompression;
		unsigned long  biSizeImage;
		long           biXPelsPerMeter;
		long           biYPelsPerMeter;
		unsigned long  biClrUsed;
		unsigned long  biClrImportant;
	} BitMapInfoHeader;

	bool saveAsBmp(const Texture * texture, const std::string & str)
	{
		int width = texture->getWidth(), height = texture->getHeight();

		int bytes = width * height * 4;

		char *data = new char[bytes];
		memset(data, 0, bytes * sizeof(char));

		for (int j = 0; j < height; ++j) {
			int offset = (height - j - 1) * width * 4;
			for (int i = 0; i < width; ++i) {
				Spectrum s = texture->sample(i, j);
				data[offset++] = color_FloatToChar(s.b);
				data[offset++] = color_FloatToChar(s.g);
				data[offset++] = color_FloatToChar(s.r);
				data[offset++] = (char)255;
			}
		}

		char bmpType[2];
		bmpType[0] = 'B';
		bmpType[1] = 'M';

		// set the bitmap header
		BitMapInfoHeader header;
		memset(&header, 0, sizeof(header));
		header.biWidth = width;
		header.biHeight = height;
		header.biSize = sizeof(header);
		header.biPlanes = 1;
		header.biBitCount = 32;
		header.biCompression = 0L;

		// file header
		BitMapFileHeader bmfh;
		memset(&bmfh, 0, sizeof(bmfh));
		bmfh.bfOffBits = sizeof(bmfh) + sizeof(header) + sizeof(bmpType);
		bmfh.bfSize = bmfh.bfOffBits + bytes;

		// open the file
		std::ofstream ofs;
		ofs.open(str, std::ios::binary);

		// if the file could not be opened , just return an error
		if (!ofs.is_open())
		{
			std::cout << "Can't open file : " << str << std::endl;
			return false;
		}

		// write
		ofs.write((const char*)bmpType, sizeof(bmpType));
		ofs.write((const char*)&bmfh, sizeof(bmfh));
		ofs.write((const char*)&header, sizeof(header));
		ofs.write(data, bytes);

		ofs.close();
		delete[] data;
		return true;
	}
}
