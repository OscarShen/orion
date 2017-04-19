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
namespace orion {

	class TexManager : public Singleton<TexManager>
	{
	public:
		static void init() { new TexManager(); }

		bool output(const Texture *texture, const std::string &name) {
			return saveAsBmp(texture, name);
		}

	private:
		TexManager() {}
	};
}

#endif // !ORION_TEXTURE_MANAGER_H_
