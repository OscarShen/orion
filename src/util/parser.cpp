#include "parser.h"
#include <core/primitive.h>
#include <integrator/whitted.h>
#include <material/matte.h>
#include <material/merlmaterial.h>
#include <material/mirror.h>
#include <material/glass.h>
#include <util/strutil.h>
#include <util/envvariable.h>
#include <util/texmanager.h>

namespace orion {

	DEFINE_SINGLETON(Parser);

	void Parser::makeRenderOption()
	{
		TiXmlElement *element = root->FirstChildElement("Resource");
		setResPath(element->Attribute("filename"));
		_makeCamera();
		_makeModel();
		_makeAccel();
		_makeIntegrator();
	}

	void orion::Parser::_makeModel()
	{
		std::vector<std::shared_ptr<Primitive>> prims;
		TiXmlElement *model = root->FirstChildElement("Model");
		while (model) {
			std::shared_ptr<MeshData> meshdata = MeshManager::inst()->loadMeshData(getResPath() + model->Attribute("filename"));
			// Transform
			ParamVec transParam;
			GET_PARAMVEC(model, transParam);
			Transform transform = createTransform(transParam);
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

	Parser::Parser(const std::string &scenepath) {
		doc = std::shared_ptr<TiXmlDocument>(new TiXmlDocument(scenepath.c_str()));
		doc->LoadFile();
		CHECK_INFO(!doc->Error(), doc->ErrorDesc());
		root = doc->RootElement();
		renderOption.reset(new RenderOption());
	}

}
