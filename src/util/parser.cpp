#include "parser.h"
#include <core/primitive.h>
#include <integrator/whitted.h>
#include <util/strutil.h>
#include <util/envvariable.h>

namespace orion {

	DEFINE_SINGLETON(Parser);

	void Parser::makeRenderOption()
	{
		TiXmlElement *element = root->FirstChildElement("Resource");
		setResPath(element->GetText());
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
			auto scaleNode = model->FirstChildElement("Scale");
			Vector3f scaleVec = parseVector3f(scaleNode->GetText());
			auto rotateNode = model->FirstChildElement("Rotate");
			Vector3f rotateVec = parseVector3f(rotateNode->GetText());
			auto translateNode = model->FirstChildElement("Translate");
			Vector3f translateVec = parseVector3f(translateNode->GetText());

			Transform transform = translate(translateVec) * rotate(rotateVec) * scale(scaleVec);

			Transform *t, *invt;
			TransformCache::inst()->lookup(transform, &t, &invt);
			std::vector<std::shared_ptr<Shape>> shapes = createTriangleMesh(t, invt, meshdata);
			// Material
			// TODO

			for (auto &item : shapes) {
				prims.push_back(std::shared_ptr<Primitive>(new Primitive(item)));
			}
			renderOption->prims.insert(renderOption->prims.end(), prims.begin(), prims.end());

			model = model->NextSiblingElement("Model");
		}
	}

	void Parser::_makeAccel()
	{
		TiXmlElement *accelNode = root->FirstChildElement("Accelerator");
		std::string accel(accelNode->FirstChildElement("Type")->GetText());
		trim(accel);
		if (accel == "kdtree") {
			renderOption->accel.reset(new KdTreeAccel(renderOption->prims)); // default KDtree

		}else{
			CHECK_INFO(false, "Not support now!");
		}
	}

	void Parser::_makeIntegrator()
	{
		TiXmlElement *inteNode = root->FirstChildElement("Integrator");
		std::string inte(inteNode->FirstChildElement("Type")->GetText());
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
				auto origNode = cameraNode->FirstChildElement("Origin");
				Point3f origin = parsePoint3f(origNode->GetText());
				auto lookatNode = cameraNode->FirstChildElement("LookAt");
				Point3f lookat = parsePoint3f(lookatNode->GetText());
				auto upNode = cameraNode->FirstChildElement("Up");
				Vector3f up = parseVector3f(upNode->GetText());
				auto filmSizeNode = cameraNode->FirstChildElement("FilmSize");
				Vector2i filmSize = parseVector2i(filmSizeNode->GetText());
				auto fovNode = cameraNode->FirstChildElement("Fov");
				Float fov = parseFloat(fovNode->GetText());
				auto film = std::make_shared<RenderTarget>(filmSize.x, filmSize.y);
				renderOption->camera.reset(new PerspectiveCamera(origin, lookat, up, fov, film));
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
