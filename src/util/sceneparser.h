/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_UTIL_SCENE_PARSER_H_
#define ORION_UTIL_SCENE_PARSER_H_
#include <orion.h>
#include <shape/shape.h>
#include <core/scene.h>
#include <common/singleton.h>
#include <core/integrator.h>
#include "transformcache.h"

#include <tinyXML/tinyxml.h>

namespace orion {

	struct RenderOption
	{
		std::vector<std::shared_ptr<Primitive>> prims;
		std::shared_ptr<Accelerator> accel;
		std::shared_ptr<Integrator> integrator;
	};

	class Parser : public Singleton<Parser>
	{
	private:
		std::shared_ptr<TiXmlDocument> doc;
		TiXmlElement *root;

	public:
		std::unique_ptr<RenderOption> renderOption; // maybe very large

	public:
		static void init(const std::string &path) { new Parser(path); }
		void makeRenderOption();

	private:
		void _makeModel();
		void _makeAccel();
		void _makeIntegrator();
		Parser(const std::string &path);
	};


}

#endif // !ORION_UTIL_SCENE_PARSER_H_
