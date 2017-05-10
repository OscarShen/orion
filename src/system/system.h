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
#include <camera/perspective.h>
#include <util/texmanager.h>
#include <util/logmanager.h>
#include <util/timer.h>
#include <util/meshmanager.h>
#include <core/scene.h>
#include <integrator/whitted.h>
namespace orion {

	class System
	{
	private:
		std::shared_ptr<Camera> camera;
		std::shared_ptr<Scene> scene;
		std::shared_ptr<Integrator> integrator;
		std::shared_ptr<Sampler> sampler;
		int nSamples;

	public:
		System() { _init(); }
		~System() { }

		void render();
		void outputFilm(const std::string &name) {
			TexManager::inst()->write(camera->getFilm(), name);
		}

		void setUp();

	private:
		void _init();
		void _pre();
		void _post();
		
	};

}


#endif // !ORION_SYSTEM_H_

