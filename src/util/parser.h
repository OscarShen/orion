/************************************************************************ 
 * @description :  build scene from xml file
 * @author		:  Oscar Shen
 * @creat 		:  2017Äê5ÔÂ5ÈÕ
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_UTIL_SCENE_PARSER_H_
#define ORION_UTIL_SCENE_PARSER_H_
#include <orion.h>
#include <core/triangle.h>
#include <core/scene.h>
#include <core/integrator.h>
#include <camera/pinhole.h>

#include <tinyxml.h>

#include "transformcache.h"

namespace orion {
#define GET_PARAMSET(node, paramset) {\
	auto paramNode = node->FirstChildElement("Param");\
	while (paramNode) {\
		auto attributeNode = paramNode->FirstAttribute();\
		while (attributeNode) {\
			paramset.setParam(attributeNode->Name(), attributeNode->Value());\
			attributeNode = attributeNode->Next();\
		}\
		paramNode = paramNode->NextSiblingElement("Param");\
	}}

#define GET_PARAMVEC(node, paramvec) {\
	auto paramNode = node->FirstChildElement("Param");\
	while (paramNode) {\
		auto attributeNode = paramNode->FirstAttribute();\
		while (attributeNode) {\
			paramvec.addParam(attributeNode->Name(), attributeNode->Value());\
			attributeNode = attributeNode->Next();\
		}\
		paramNode = paramNode->NextSiblingElement("Param");\
	}}

	struct RenderOption
	{
		std::shared_ptr<Scene> scene;
		std::shared_ptr<Camera> camera;
		std::vector<std::shared_ptr<Primitive>> prims;
		std::shared_ptr<Integrator> integrator;
		std::vector<std::shared_ptr<Light>> lights;
		std::shared_ptr<Sampler> sampler;
		Transform lookatTransform; // use for all things to match camera's direction
		int nSamples = 1;
	};

	class Parser
	{
	private:
		std::unique_ptr<RenderOption> renderOption;
		std::shared_ptr<TiXmlDocument> doc;
		TiXmlElement *root;

	public:
		Parser(const std::string &path);
		const std::unique_ptr<RenderOption>& getRenderOption() const { return renderOption; }

	private:
		void makeRenderOption();
		void _makeModel();
		void _makeIntegrator();
		void _makeCamera();
		void _makeLight();
		void _makeSampler();
		void _makeScene();
		void _makeEmvmap();
		bool _readSPDFile(const std::string &path, Spectrum *s);
		std::shared_ptr<Material> _makeMaterial(TiXmlElement *matNode);
		std::shared_ptr<Texture> _makeTexture(const ParamSet &param);
	};
}

#endif // !ORION_UTIL_SCENE_PARSER_H_
