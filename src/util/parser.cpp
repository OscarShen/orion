#include "parser.h"
#include <core/primitive.h>
#include <shape/disk.h>
#include <shape/sphere.h>
#include <integrator/whitted.h>
#include <integrator/directlighting.h>
#include <integrator/pathtracing.h>
#include <light/light.h>
#include <light/arealight.h>
#include <camera/thinlens.h>
#include <texture/checkboard.h>
#include <texture/constant.h>
#include <texture/floattexture.h>
#include <texture/gridtexture.h>
#include <texture/imagetexture.h>
#include <material/matte.h>
#include <material/merlmaterial.h>
#include <material/mirror.h>
#include <material/glass.h>
#include <material/plastic.h>
#include <sampler/sampler.h>
#include <sampler/sobol.h>
#include <util/strutil.h>
#include <util/envvariable.h>
#include <util/texmanager.h>
#include <util/meshmanager.h>
#include <util/materialmanager.h>

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
		_makeScene();
	}

	void orion::Parser::_makeModel()
	{
		std::vector<std::shared_ptr<Primitive>> prims;
		std::shared_ptr<Material> material;

		TiXmlElement *model = root->FirstChildElement("Model");
		while (model) {
			std::vector<std::shared_ptr<Shape>> shapes;
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
			bool hasObjMaterial = false;
			if (modeltype == "triMesh") {
				std::shared_ptr<ModelData> modeldata = MeshManager::inst()->loadMeshData(getResPath() + model->Attribute("filename"));
				auto &data = *modeldata;
				if (data[0].matName != "DefaultMaterial")
					hasObjMaterial = true;

				// Shapes
				for (size_t i = 0; i < modeldata->size(); ++i) {
					shapes = createTriangleMesh(t, invt, data[i]);
					if (hasObjMaterial) {
						material = MaterialManager::inst()->getMaterial(data[i].matName);
						for (auto &shape : shapes) {
							prims.push_back(std::shared_ptr<Primitive>(new Primitive(shape, material)));
						}
					}
				}
			}
			else if (modeltype == "disk") {
				ParamSet ps;
				GET_PARAMSET(model, ps);
				shapes.push_back(createDisk(t, invt, ps));
			}
			else if (modeltype == "sphere") {
				ParamSet ps;
				GET_PARAMSET(model, ps);
				shapes.push_back(createSphere(t, invt, ps));
			}
			else if (modeltype == "triMeshHandy") {
				ParamSet ps;
				GET_PARAMSET(model, ps);
				std::string pStr = ps.getParam("p"), indicesStr = ps.getParam("indices");
				std::string uvStr, nStr;
				if (ps.hasParam("uv")) { uvStr = ps.getParam("uv"); }
				if (ps.hasParam("n")) { nStr = ps.getParam("n"); }
				auto meshdata = parseMeshData(pStr, uvStr, nStr, indicesStr);
				shapes = createTriangleMesh(t, invt, *meshdata);
			}

			// Material
			if (!hasObjMaterial) {
				TiXmlElement *mat = model->FirstChildElement("Material");
				material = _makeMaterial(mat);
			}

			// area light
			// obj file does not have lightsource
			TiXmlElement *areaLightNode = model->FirstChildElement("AreaLight");
			if (areaLightNode) {
				ParamSet areaParam;
				GET_PARAMSET(areaLightNode, areaParam);
				for (auto &item : shapes) {
					std::shared_ptr<AreaLight> areaLight = createAreaLight(transform, item, areaParam);
					renderOption->lights.push_back(areaLight);
					prims.push_back(std::shared_ptr<Primitive>(new Primitive(item, material, areaLight)));
				}
			}
			else {
				if(!hasObjMaterial)
					for (auto &item : shapes)
						prims.push_back(std::shared_ptr<Primitive>(new Primitive(item, material)));
			}
			renderOption->prims.insert(renderOption->prims.end(), prims.begin(), prims.end());
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
		ParamSet ps;
		GET_PARAMSET(inteNode, ps);
		if (inte == "whitted") {
			renderOption->integrator = createWhittedIntegrator(renderOption->camera, renderOption->sampler, ps);
		}
		else if (inte == "directLighting") {
			renderOption->integrator = createDirectLightingIntegrator(renderOption->camera, renderOption->sampler, ps);
		}
		else if (inte == "pathtracing") {
			renderOption->integrator = createPathTracingIntegrator(renderOption->camera, renderOption->sampler, ps);
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
			renderOption->nSamples = parseInt(camParam.getParam("nSamples"));

			// Transform
			TiXmlElement *transNode = cameraNode->FirstChildElement("Transform");
			Transform transform;
			if (transNode) {
				ParamVec transParam;
				GET_PARAMVEC(transNode, transParam);
				transform = createTransform(transParam);
			}

			if (cam == "perspective") {
				renderOption->camera = createPerspectiveCamera(transform, camParam);
			}
			else if (cam == "thinlens") {
				renderOption->camera = createThinLensCamera(transform, camParam);
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
				renderOption->sampler = createPseudoSampler();
			}
			else if (type == "sobol")
				renderOption->sampler = createSobolSampler();
			else {
				CHECK_INFO(false, "Not support now!");
			}
		}
	}

	void Parser::_makeScene()
	{
		renderOption->scene.reset(new Scene(renderOption->accel, renderOption->lights));

		// erase resource which won't be used.
		renderOption->prims.erase(renderOption->prims.begin(), renderOption->prims.end());
		renderOption->lights.erase(renderOption->lights.begin(), renderOption->lights.end());
	}

	void Parser::_loadMaterial(const std::string & filename)
	{
		std::ifstream matIstream(filename.c_str());
		std::vector<material_t> mtls;
		LoadMtl(mtls, matIstream);

		for (auto &mtl_t : mtls) {
			// uber material
		}
	}

	std::shared_ptr<Material> Parser::_makeMaterial(TiXmlElement * matNode)
	{
		std::shared_ptr<Material> material;
		if (matNode) {
			const char *matC = matNode->Attribute("type");
			std::string mattype = matC ? matC : std::string();
			const char *matN = matNode->Attribute("name");
			std::string matName = matN ? matN : std::string();
			ParamSet matParam;
			GET_PARAMSET(matNode, matParam);

			// matte
			const char *namedC = matNode->Attribute("named");
			std::string named = namedC ? namedC : std::string();
			if (!named.empty()) {
				material = MaterialManager::inst()->getMaterial(named);
			}
			else if (mattype == "matte") {
				TiXmlElement *kdNode = matNode->FirstChildElement("Kd");
				ParamSet kdParam;
				std::shared_ptr<Texture> kd;
				if (kdNode) {
					GET_PARAMSET(kdNode, kdParam);
					kd = _makeTexture(kdParam);
				}

				TiXmlElement *sigmaNode = matNode->FirstChildElement("sigma");
				std::shared_ptr<Texture> sigma;
				if (sigmaNode) {
					ParamSet sigmaParam;
					GET_PARAMSET(sigmaNode, sigmaParam);
					sigma = _makeTexture(sigmaParam);
				}

				material = createMatteMaterial(kd, sigma);
			}
			else if (mattype == "plastic") {
				TiXmlElement *kdNode = matNode->FirstChildElement("Kd");
				ParamSet kdParam;
				std::shared_ptr<Texture> kd;
				if (kdNode) {
					GET_PARAMSET(kdNode, kdParam);
					kd = _makeTexture(kdParam);
				}

				TiXmlElement *ksNode = matNode->FirstChildElement("Ks");
				ParamSet ksParam;
				std::shared_ptr<Texture> ks;
				if (ksNode) {
					GET_PARAMSET(ksNode, ksParam);
					ks = _makeTexture(ksParam);
				}

				TiXmlElement *roughnessNode = matNode->FirstChildElement("roughness");
				ParamSet roughnessParam;
				std::shared_ptr<Texture> roughness;
				if (roughnessNode) {
					GET_PARAMSET(roughnessNode, roughnessParam);
					roughness = _makeTexture(roughnessParam);
				}
				
				material = createPlasticMaterial(kd, ks, roughness);
			}
			else if (mattype == "merl") {
				material = createMerlMaterial(matParam);
			}
			else if (mattype == "mirror") {
				material = createMirrorMaterial();
			}
			else if (mattype == "glass") {
				material = createGlassMaterial(matParam);
			}

			if (!matName.empty())
				MaterialManager::inst()->addMaterial(matName, material);
		}
		CHECK_INFO(material != nullptr, "null material!");
		return material;
	}

	std::shared_ptr<Texture> Parser::_makeTexture(const ParamSet & param)
	{
		std::string type = param.getParam("type");
		std::shared_ptr<Texture> tex;
		trim(type);
		if (type == "image") {
			std::string filename = param.getParam("filename");
			if (!filename.empty())
				tex = TexManager::inst()->read(filename);
		}
		else if (type == "checkbox") {
			std::string color0Str = param.getParam("color0");
			std::string color1Str = param.getParam("color1");
			if (!color0Str.empty() && !color1Str.empty()) {
				Spectrum color0 = parseSpectrum(color0Str);
				Spectrum color1 = parseSpectrum(color1Str);
				tex.reset(new CheckBoardTexture(color0, color1));
			}
		}
		else if (type == "constant") {
			std::string colorStr = param.getParam("color");
			if (!colorStr.empty()) {
				Spectrum color = parseSpectrum(colorStr);
				tex.reset(new ConstantTexture(color));
			}
		}
		else if (type == "float") {
			std::string valueStr = param.getParam("value"); // only constant float now !!
			if (!valueStr.empty()) {
				Float value = parseFloat(valueStr);
				tex.reset(new FloatTexture(value));
			}
		}

		CHECK_INFO(tex != nullptr, "can't make texture because lack of parameters!");
		return tex;
	}

	Parser::Parser(const std::string &scenepath) {
		doc = std::shared_ptr<TiXmlDocument>(new TiXmlDocument(scenepath.c_str()));
		doc->LoadFile();
		CHECK_INFO(!doc->Error(), doc->ErrorDesc());
		root = doc->RootElement();
		CHECK_INFO(root != nullptr, "xml root node is null pointer!");
		renderOption.reset(new RenderOption());
	}

}
