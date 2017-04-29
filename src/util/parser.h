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
#include <common/singleton.h>
#include <shape/shape.h>
#include <core/scene.h>
#include <core/integrator.h>
#include <camera/perspective.h>

#include <tinyXML/tinyxml.h>

#include "transformcache.h"

namespace orion {

	struct RenderOption
	{
		std::shared_ptr<Camera> camera;
		std::vector<std::shared_ptr<Primitive>> prims;
		std::shared_ptr<Accelerator> accel;
		std::shared_ptr<Integrator> integrator;
	};

	class Parser : public Singleton<Parser>
	{
	private:
		std::unique_ptr<RenderOption> renderOption;
		std::shared_ptr<TiXmlDocument> doc;
		TiXmlElement *root;

	public:
		static void init(const std::string &path) { new Parser(path); }
		const std::unique_ptr<RenderOption>& getRenderOption() const { return renderOption; }
		void makeRenderOption();

	private:
		void _makeModel();
		void _makeAccel();
		void _makeIntegrator();
		void _makeCamera();
		Parser(const std::string &path);
	};


}

#endif // !ORION_UTIL_SCENE_PARSER_H_
