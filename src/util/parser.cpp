#include "parser.h"
#include <core/primitive.h>
#include <integrator/whitted.h>
#include <light/light.h>
#include <material/matte.h>
#include <material/merlmaterial.h>
#include <material/mirror.h>
#include <material/glass.h>
#include <util/strutil.h>
#include <util/envvariable.h>
#include <util/texmanager.h>
#include <util/meshmanager.h>

namespace orion {

	DEFINE_SINGLETON(Parser);

	void Parser::makeRenderOption()
	{
		TiXmlElement *element = root->FirstChildElement("Resource");
		setResPath(element->Attribute("filename"));
		_makeCamera();
		_makeModel();
		_makeAccel();
		_makeLight();
		_makeIntegrator();
	}

	void orion::Parser::_makeModel()
	{
		std::vector<std::shared_ptr<Primitive>> prims;
		TiXmlElement *model = root->FirstChildElement("Model");
		while (model) {
			std::shared_ptr<MeshData> meshdata = MeshManager::inst()->loadMeshData(getResPath() + model->Attribute("filename"));
			// Transform
			TiXmlElement *transNode = model->FirstChildElement("Transform");
			Transform transform;
			if (transNode) {
				ParamVec transParam;
				GET_PARAMVEC(transNode, transParam);
				transform = createTransform(transParam);
			}
			Transform *t, *invt;
			TransformCache::inst()->lookup(transform, &t, &invt);

			// Shapes
			std::vector<std::shared_ptr<Shape>> shapes = createTriangleMesh(t, invt, meshdata);

			// Material
			std::shared_ptr<Material> material;
			auto mat = model->FirstChildElement("Material");
			if (mat) {
				std::string mattype = mat->Attribute("type");
				ParamSet matParam;
				GET_PARAMSET(mat, matParam);
				if (mattype == "matte") {
					material = createMatteMaterial(matParam);
				}
				else if (mattype == "merl") {
					material = createMerlMaterial(matParam);
				}
				else if (mattype == "mirror") {
					material = createMirrorMaterial(matParam);
				}
				else if (mattype == "glass") {
					material = createGlassMaterial(matParam);
				}
			}

			// Primitives
			for (auto &item : shapes) {
				prims.push_back(std::shared_ptr<Primitive>(new Primitive(item, material)));
			}
			renderOption->prims.insert(renderOption->prims.end(), prims.begin(), prims.end());
			prims.clear();
			// Next
			model = model->NextSiblingElement("Model");
		}
	}

	void Parser::_makeAccel()
	{
		TiXmlElement *accelNode = root->FirstChildElement("Accelerator");
		std::string accel = accelNode->Attribute("type");
		trim(accel);
		if (accel == "kdtree") {
			renderOption->accel = createKdTreeAccelerator(renderOption->prims);
		}else{
			CHECK_INFO(false, "Not support now!");
		}
	}

	void Parser::_makeIntegrator()
	{
		TiXmlElement *inteNode = root->FirstChildElement("Integrator");
		std::string inte = inteNode->Attribute("type");
		trim(inte);
		if (inte == "whitted") {
			renderOption->integrator.reset(new WhittedIntegrator());
		}
		else {
			CHECK_INFO(false, "Not support now!");
		}
	}

	void Parser::_makeCamera()
	{
		TiXmlElement *cameraNode = root->FirstChildElement("Camera");

		if (cameraNode) {
			std::string cam = cameraNode->Attribute("type");
			trim(cam);
			if (cam == "perspective") {
				ParamSet camParam;
				GET_PARAMSET(cameraNode, camParam);
				renderOption->camera = createPerspectiveCamera(camParam);
			}
		}
		else {
			CHECK_INFO(false, "Not support now!");
		}
	}

	void Parser::_makeLight()
	{
		TiXmlElement *lightNode = root->FirstChildElement("Light");
		while (lightNode) {
			std::string type = lightNode->Attribute("type");
			trim(type);
			ParamSet ps;
			GET_PARAMSET(lightNode, ps);
			if (type == "point") {
				TiXmlElement *transNode = lightNode->FirstChildElement("Transform");
				Transform t;
				if (transNode) {
					ParamVec pv;
					GET_PARAMVEC(transNode, pv);
					t = createTransform(pv);
				}
				std::shared_ptr<Light> light = createPointLight(t, ps);
				renderOption->lights.push_back(light);
			}

			lightNode = lightNode->NextSiblingElement("Light");
		}
	}

	Parser::Parser(const std::string &scenepath) {
		doc = std::shared_ptr<TiXmlDocument>(new TiXmlDocument(scenepath.c_str()));
		doc->LoadFile();
		CHECK_INFO(!doc->Error(), doc->ErrorDesc());
		root = doc->RootElement();
		renderOption.reset(new RenderOption());
	}

}
