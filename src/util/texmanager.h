/************************************************************************ 
 * @description :  texture manager
 * @author		:  Oscar Shen
 * @creat 		:  2017-4-19 10:45:42
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_TEXTURE_MANAGER_H_
#define ORION_TEXTURE_MANAGER_H_
#include <orion.h>
#include <common/singleton.h>
#include <texture/texture.h>
#include <texture/imagetexture.h>
#include <io/imageio.h>
namespace orion {

	class TexManager : public Singleton<TexManager>
	{
	private:
		std::map<std::string, std::shared_ptr<ImageMemory>> images;

	public:
		static void init() { new TexManager(); }
		void clear();

		std::shared_ptr<ImageTexture> read(const std::string &name);

		bool write(std::shared_ptr<Texture> texture, const std::string &name) {
			return saveAsBmp(texture.get(), name);
		}

		TexManager() {}
	};
}

#endif // !ORION_TEXTURE_MANAGER_H_
