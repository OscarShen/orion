#include "sceneparser.h"
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
			Transform transform;
			TiXmlElement *transNode = model->FirstChildElement("Transform");
			while (transNode) {
				transform = parseTransform(transNode->GetText()) * transform;
				transNode = transNode->NextSiblingElement("Transform");
			}
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
		if (accelNode) {
			std::string accel(accelNode->GetText());
			trim(accel);
			// other accel
		}else{
			renderOption->accel.reset(new KdTreeAccel(renderOption->prims)); // default KDtree
		}
	}

	void Parser::_makeIntegrator()
	{
		TiXmlElement *inteNode = root->FirstChildElement("Integrator");
		if (inteNode) {
			std::string inte(inteNode->GetText());
			trim(inte);
			// other accel
		}
		else {
			renderOption->integrator.reset(new WhittedIntegrator());
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
