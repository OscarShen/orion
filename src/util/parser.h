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
#include <common/singleton.h>
#include <shape/shape.h>
#include <core/scene.h>
#include <integrator/integrator.h>
#include <camera/perspective.h>

#include <tinyXML/tinyxml.h>

#include "transformcache.h"

namespace orion {
	typedef struct {
		std::string name;

		float ambient[3];
		float diffuse[3];
		float specular[3];
		float transmittance[3];
		float emission[3];
		float shininess;
		float ior;      // index of refraction
		float dissolve; // 1 == opaque; 0 == fully transparent
						// illumination model (see http://www.fileformat.info/format/material/)
		int illum;

		int dummy; // Suppress padding warning.

		std::string ambient_texname;            // map_Ka
		std::string diffuse_texname;            // map_Kd
		std::string specular_texname;           // map_Ks
		std::string specular_highlight_texname; // map_Ns
		std::string bump_texname;               // map_bump, bump
		std::string displacement_texname;       // disp
		std::string alpha_texname;              // map_d
		std::map<std::string, std::string> unknown_parameter;
	} material_t;
	void LoadMtl(std::vector<material_t> &materials, std::istream &inStream);

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
		std::shared_ptr<Accelerator> accel;
		std::shared_ptr<Integrator> integrator;
		std::vector<std::shared_ptr<Light>> lights;
		std::shared_ptr<Sampler> sampler;
		int nSamples = 1;
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
		void _makeLight();
		void _makeSampler();
		void _makeScene();
		void _loadMaterial(const std::string &filename);
		std::shared_ptr<Material> _makeMaterial(TiXmlElement *matNode);
		std::shared_ptr<Texture> _makeTexture(const ParamSet &param);
		Parser(const std::string &path);
	};
}

#endif // !ORION_UTIL_SCENE_PARSER_H_
