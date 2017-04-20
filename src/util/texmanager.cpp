#include "texmanager.h"

namespace orion {

	DEFINE_SINGLETON(TexManager);

	void TexManager::clear()
	{
		for (auto it = images.begin(); it != images.end();) {
			it = images.erase(it);
		}
	}

	std::shared_ptr<ImageTexture> TexManager::read(const std::string & name)
	{
		std::shared_ptr<ImageTexture> texture(new ImageTexture());
		auto it = images.find(name);
		if (it != images.end()) { // already exists.
			texture->memory = it->second;
			texture->width = it->second->width;
			texture->height = it->second->height;
			return texture;
		}

		std::shared_ptr<ImageMemory> mem;
		bool b = loadImage(name, mem);
		CHECK_INFO(b, "image load failed!");
		images.insert(std::pair<std::string, std::shared_ptr<ImageMemory>>(name, mem));
		texture->memory = mem;
		texture->width = mem->width;
		texture->height = mem->height;
		return texture;
	}

}