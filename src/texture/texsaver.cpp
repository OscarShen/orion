#include "texsaver.h"

namespace ort {

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
		// temp data, those information should be given by texture
		int width = 800, height = 600;

		int bytes = width * height * 4;

		char *data = new char[bytes];
		memset(data, 0, bytes * sizeof(char));

		// TODO : fill texture here


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
		std::cout << "bmpType size : " << sizeof(bmpType) << std::endl;
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