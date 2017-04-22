/************************************************************************ 
 * @description :  system class, responsible for system control
 * @author		:  Oscar Shen
 * @creat 		:  2017-4-22 11:57:25
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_SYSTEM_H_
#define ORION_SYSTEM_H_
#include <orion.h>
#include <texture/rendertarget.h>
#include <camera/camera.h>
#include <util/texmanager.h>
#include <util/logmanager.h>
#include <util/timer.h>
namespace orion {

	class System
	{
	private:
		std::shared_ptr<Camera> camera;

	public:
		System() { _init(); }
		~System() { }

		void render();
		void outputFilm(const std::string &name) {
			TexManager::inst()->write(camera->getFilm(), name);
		}

		void setCamera(const std::shared_ptr<Camera> &camera) { this->camera = camera; }

	private:
		void _init();
	};

}


#endif // !ORION_SYSTEM_H_

