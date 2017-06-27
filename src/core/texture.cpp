#include "texture.h"

namespace orion {
	Spectrum Texture::evaluate(const Intersection * isec) const
	{
		if(isec)
			return sample((int)(isec->uv.x * width), (int)(isec->uv.y * height));
		return Spectrum();
	}
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

	inline char color_FloatToChar(Float f) {
		f = clamp(f);
		return (char)(f * 255);
	}

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