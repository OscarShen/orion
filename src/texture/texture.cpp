#include "texture.h"

namespace ort {
	bool Texture::output(const std::string & name, SaveFormat format)
	{
		switch (format)
		{
		case SaveFormat::bmp:
		{
			return saveAsBmp(this, name);
			break;
		}
		default:
		{
			std::cout << "do not support this format." << std::endl;
			return false;
			break;
		}
		}
	}
}