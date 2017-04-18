#include "texture.h"

namespace orion {
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
			ERROR("do not support this format.");
			return false;
			break;
		}
		}
	}
}