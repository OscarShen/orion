#include "parser.h"
#include <core/primitive.h>
#include <shape/disk.h>
#include <integrator/whitted.h>
#include <light/light.h>
#include <camera/thinlens.h>
#include <material/matte.h>
#include <material/merlmaterial.h>
#include <material/mirror.h>
#include <material/glass.h>
#include <sampler/pseudo.h>
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
		_makeSampler();
	}

	void orion::Parser::_makeModel()
	{
		std::vector<std::shared_ptr<Primitive>> prims;
		TiXmlElement *model = root->FirstChildElement("Model");
		while (model) {
			std::vector<std::shared_ptr<Shape>> shapes;
			ParamSet ps;
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

			// shape
			std::string modeltype = model->Attribute("type");
			if (modeltype == "triangleMesh") {
				std::shared_ptr<MeshData> meshdata = MeshManager::inst()->loadMeshData(getResPath() + model->Attribute("filename"));
				// Shapes
				shapes = createTriangleMesh(t, invt, meshdata);
			}
			else if (modeltype == "disk") {
				GET_PARAMSET(model, ps);
				shapes.push_back(createDisk(t, invt, ps));
			}

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
			ParamSet camParam;
			GET_PARAMSET(cameraNode, camParam);
			renderOption->nSamples = parseFloat(camParam.getParam("nSamples"));
			if (cam == "perspective") {
				renderOption->camera = createPerspectiveCamera(camParam);
			}
			else if (cam == "thinlens") {
				renderOption->camera = createThinLensCamera(camParam);
			}
			else {
				CHECK_INFO(false, "Not support now!");
			}
		}
		else {
			CHECK_INFO(false, "No camera!");
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
				renderOption->lights.push_back(createPointLight(ps));
			}
			else if (type == "spot") {
				renderOption->lights.push_back(createSpotLight(ps));
			}
			else if (type == "distant") {
				renderOption->lights.push_back(createDistantLight(ps));
			}

			lightNode = lightNode->NextSiblingElement("Light");
		}
	}

	void Parser::_makeSampler()
	{
		TiXmlElement *samplerNode = root->FirstChildElement("Sampler");
		if (samplerNode) {
			std::string type = samplerNode->Attribute("type");
			ParamSet ps;
			GET_PARAMSET(samplerNode, ps);
			if (type == "pseudo") {
				renderOption->rand.reset(new RandomSequence(createPseudoSampler(), 0)); // pseudo sampler does not need instance.
			}
			else {
				CHECK_INFO(false, "Not support now!");
			}
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
