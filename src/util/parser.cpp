#include "parser.h"
#include <core/primitive.h>
#include <core/floatfile.h>
#include <integrator/path.h>
#include <texture/constant.h>
#include <texture/floattexture.h>
#include <texture/imagetexture.h>
#include <material/matte.h>
#include <material/mirror.h>
#include <material/glass.h>
#include <material/plastic.h>
#include <material/metal.h>
#include <sampler/sobol.h>
#include <sampler/pseudo.h>
#include <light/diffuse.h>
#include <light/distant.h>
#include <light/point.h>
#include <light/spot.h>
#include <light/envmap.h>
#include <camera/pinhole.h>
#include <util/strutil.h>
#include <util/envvariable.h>
#include <util/param.h>

namespace orion {

	void Parser::makeRenderOption()
	{
		TiXmlElement *element = root->FirstChildElement("Resource");
		setResPath(element->Attribute("filename"));
		_makeCamera();
		_makeModel();
		_makeLight();
		_makeSampler();
		_makeIntegrator();
		_makeEmvmap();
		_makeScene();
	}

	void orion::Parser::_makeModel()
	{
		std::shared_ptr<Material> material;

		TiXmlElement *model = root->FirstChildElement("Model");
		while (model) {
			std::vector<std::shared_ptr<Primitive>> prims;
			std::vector<std::shared_ptr<Triangle>> shapes;
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
			if (modeltype == "mesh") {
				auto mesh = std::make_shared<Mesh>();
				std::string fName = getResPath() + model->Attribute("filename");
				mesh->loadMesh(fName);
				shapes = createTriangleMesh(t, invt, mesh);

			}
			else if (modeltype == "meshHandy") {
				ParamSet ps;
				GET_PARAMSET(model, ps);
				std::string pStr = ps.getParam("p");
				std::string uvStr, nStr;
				if (ps.hasParam("uv")) { uvStr = ps.getParam("uv"); }
				if (ps.hasParam("n")) { nStr = ps.getParam("n"); }
				auto meshdata = parseMeshData(pStr, uvStr, nStr);
				shapes = createTriangleMesh(t, invt, meshdata);
			}

			// Material
			TiXmlElement *mat = model->FirstChildElement("Material");
			material = _makeMaterial(mat);

			// area light
			// obj file does not have lightsource
			TiXmlElement *areaLightNode = model->FirstChildElement("AreaLight");
			if (areaLightNode) {
				ParamSet areaParam;
				GET_PARAMSET(areaLightNode, areaParam);
				for (auto &item : shapes) {
					std::shared_ptr<AreaLight> areaLight = createDiffuseAreaLight(transform, item, areaParam);
					renderOption->lights.push_back(areaLight);
					prims.push_back(std::make_shared<Primitive>(item, material, areaLight));
				}
			}
			else {
				for(auto &item : shapes)
					prims.push_back(std::make_shared<Primitive>(item, material));
			}
			renderOption->prims.insert(renderOption->prims.end(), prims.begin(), prims.end());
			// Next
			model = model->NextSiblingElement("Model");
		}
	}

	void Parser::_makeIntegrator()
	{
		TiXmlElement *inteNode = root->FirstChildElement("Integrator");
		std::string inte = inteNode->Attribute("type");
		trim(inte);
		ParamSet ps;
		GET_PARAMSET(inteNode, ps);
		if (inte == "path") {
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

			// Transform
			TiXmlElement *transNode = cameraNode->FirstChildElement("Transform");
			Transform transform;
			if (transNode) {
				ParamVec transParam;
				GET_PARAMVEC(transNode, transParam);
				transform = createTransform(transParam);
			}

			renderOption->lookatTransform = transform;

			if (cam == "pinhole") {
				renderOption->camera = createPinholeCamera(transform, camParam);
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
		renderOption->scene.reset(new Scene(renderOption->prims, renderOption->lights));

		// erase resource which won't be used.
		renderOption->prims.erase(renderOption->prims.begin(), renderOption->prims.end());
		renderOption->lights.erase(renderOption->lights.begin(), renderOption->lights.end());
	}

	void Parser::_makeEmvmap()
	{
		std::shared_ptr<Envmap> envmap;
		TiXmlElement *envmapNode = root->FirstChildElement("Envmap");
		while (envmapNode) { // may have many envmap
			// Transform
			TiXmlElement *transNode = envmapNode->FirstChildElement("Transform");
			Transform transform;
			if (transNode) {
				ParamVec transParam;
				GET_PARAMVEC(transNode, transParam);
				transform = createTransform(transParam);
			}

			ParamSet ps;
			GET_PARAMSET(envmapNode, ps);
			auto envmap = createEnvMap(transform, ps);
			renderOption->lights.push_back(envmap);

			envmapNode = envmapNode->NextSiblingElement("Envmap");
		}
	}

	bool Parser::_readSPDFile(const std::string & path, Spectrum * s)
	{
		std::vector<Float> vals;
		if (readFloatFile(path.c_str(), &vals)) {
			std::vector<Float> wls, v;
			for (size_t j = 0; j < vals.size() / 2; ++j) {
				wls.push_back(vals[2 * j]);
				v.push_back(vals[2 * j + 1]);
			}
			*s = Spectrum::fromSampled(&wls[0], &v[0], wls.size());
			return true;
		}
		else {
			*s = Spectrum(0);
			return false;
		}
	}

	std::shared_ptr<Material> Parser::_makeMaterial(TiXmlElement * matNode)
	{
		std::shared_ptr<Material> material;
		if (matNode) {
			const char *matC = matNode->Attribute("type");
			std::string mattype = matC ? matC : std::string();
			static std::map<std::string, std::shared_ptr<Material>> matStorage;

			// matte
			const char *namedC = matNode->Attribute("named");
			std::string named = namedC ? namedC : std::string();
			if (!named.empty()) {
				auto it = matStorage.find(named);
				if (it != matStorage.end())
					material = it->second;
				else
					CHECK_INFO(false, "no named material in material storage.")
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
				std::shared_ptr<FloatTexture> sigma;
				if (sigmaNode) {
					ParamSet sigmaParam;
					GET_PARAMSET(sigmaNode, sigmaParam);
					sigma = std::dynamic_pointer_cast<FloatTexture>(_makeTexture(sigmaParam));
				}

				material = createMatteMaterial(kd, sigma);
			}
			else if (mattype == "mirror") {
				TiXmlElement *KrNode = matNode->FirstChildElement("Kr");
				ParamSet krParam;
				std::shared_ptr<Texture> kr;
				if (KrNode) {
					GET_PARAMSET(KrNode, krParam);
					kr = _makeTexture(krParam);
				}
				
				material = createMirrorMaterial(kr);
			}
			else if (mattype == "glass") {
				TiXmlElement *KtNode = matNode->FirstChildElement("Kt");
				ParamSet ktParam;
				std::shared_ptr<Texture> kt;
				if (KtNode) {
					GET_PARAMSET(KtNode, ktParam);
					kt = _makeTexture(ktParam);
				}

				TiXmlElement *etaNode = matNode->FirstChildElement("eta");
				std::shared_ptr<FloatTexture> eta;
				if (etaNode) {
					ParamSet etaParam;
					GET_PARAMSET(etaNode, etaParam);
					eta = std::dynamic_pointer_cast<FloatTexture>(_makeTexture(etaParam));
				}

				material = createGlassMaterial(kt, eta);
			}
			else if (mattype == "plastic") {
				TiXmlElement *KdNode = matNode->FirstChildElement("Kd");
				ParamSet kdParam;
				std::shared_ptr<Texture> kd;
				if (KdNode) {
					GET_PARAMSET(KdNode, kdParam);
					kd = _makeTexture(kdParam);
				}

				TiXmlElement *KsNode = matNode->FirstChildElement("Ks");
				ParamSet ksParam;
				std::shared_ptr<Texture> ks;
				if (KsNode) {
					GET_PARAMSET(KsNode, ksParam);
					ks = _makeTexture(ksParam);
				}

				TiXmlElement *roughnessNode = matNode->FirstChildElement("roughness");
				ParamSet roughnessParam;
				std::shared_ptr<FloatTexture> roughness;
				if (roughnessNode) {
					GET_PARAMSET(roughnessNode, roughnessParam);
					roughness = std::dynamic_pointer_cast<FloatTexture>(_makeTexture(roughnessParam));
				}

				material = createPlasticMaterial(kd, ks, roughness);
			}
			else if (mattype == "metal") {

				TiXmlElement *etaNode = matNode->FirstChildElement("eta");
				ParamSet etaParam;
				std::shared_ptr<Texture> eta;
				if (etaNode) {
					GET_PARAMSET(etaNode, etaParam);
					if (etaParam.hasParam("SPD")) {
						std::vector<Float> vals;
						std::string path = getResPath() + "/" + etaParam.getParam("SPD");
						Spectrum seta;
						CHECK_INFO(_readSPDFile(path, &seta), "Unable to read SPD file : " << path);
						eta = std::make_shared<ConstantTexture>(seta);
					}
					else
						eta = _makeTexture(etaParam);
				}

				TiXmlElement *kNode = matNode->FirstChildElement("k");
				ParamSet kParam;
				std::shared_ptr<Texture> k;
				if (kNode) {
					GET_PARAMSET(kNode, kParam);
					if (kParam.hasParam("SPD")) {
						std::vector<Float> vals;
						std::string path = getResPath() + "/" + kParam.getParam("SPD");
						Spectrum sk;
						CHECK_INFO(_readSPDFile(path, &sk), "Unable to read SPD file : " << path);
						k = std::make_shared<ConstantTexture>(sk);
					}
					else
						k = _makeTexture(kParam);
				}

				TiXmlElement *roughnessNode = matNode->FirstChildElement("roughness");
				ParamSet roughnessParam;
				std::shared_ptr<FloatTexture> roughness;
				if (roughnessNode) {
					GET_PARAMSET(roughnessNode, roughnessParam);
					roughness = std::dynamic_pointer_cast<FloatTexture>(_makeTexture(roughnessParam));
				}

				ParamSet matParam; // you can use name of usual metals
				GET_PARAMSET(matNode, matParam);
				material = createMetalMaterial(eta, k, roughness, matParam);
			}

			// get material already defined
			const char *matN = matNode->Attribute("name");
			std::string matName = matN ? matN : std::string();
			if (!matName.empty())
				matStorage.insert(std::make_pair(matName, material));
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
			if (!filename.empty()) {
				auto im = loadImage(filename);
				tex.reset(new ImageTexture(im));
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

		return tex;
	}

	Parser::Parser(const std::string &scenepath) {
		doc = std::shared_ptr<TiXmlDocument>(new TiXmlDocument(scenepath.c_str()));
		doc->LoadFile();
		CHECK_INFO(!doc->Error(), doc->ErrorDesc());
		root = doc->RootElement();
		CHECK_INFO(root != nullptr, "xml root node is null pointer!");
		renderOption.reset(new RenderOption());
		makeRenderOption();
	}

}
