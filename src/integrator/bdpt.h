/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_INTEGRATOR_BDPT_H_
#define ORION_INTEGRATOR_BDPT_H_
#include <orion.h>
#include "integrator.h"
namespace orion {

	// path tracing vertex data
	struct Vertex
	{
		Point3f		p;
		Normal3f	n;
		Vector3f	wi;
		Vector3f	wo;
		Float		pdf;
		const Primitive *primitive;
		std::shared_ptr<BSDF> bsdf; // you can not save a simple pointer, because you can't always hold simple pointer!
		Intersection isec;
	};

	struct Contribution {
		float x, y;
		int path_length;
		Spectrum c;

		Contribution() { };

		Contribution(float x, float y, int path_length, Spectrum c) :
			x(x), y(y), path_length(path_length), c(c) { }
	};

	struct PathContribution {
		std::vector<Contribution> contributions;
		Float scaling = 1.0f;
		double total_contribution = 0.0;

		void push_back(const Contribution &c) {
			contributions.push_back(c);
			// We assume that luminance is used for SC...
			total_contribution += c.c.intensity();
		}
		bool empty() {
			return contributions.empty();
		}
		double get_total_contribution() {
			return total_contribution;
		}
		void set_scaling(Float scaling) {
			this->scaling = scaling;
		}
		Float get_scaling() const {
			return scaling;
		}
	};

	typedef std::vector<Vertex> Path;

	class BDPathTracing : public Integrator
	{
	private:
		int maxDepth;
		std::shared_ptr<Distribution1D> lightDistrib;

	public:
		BDPathTracing(const std::shared_ptr<Camera> &camera,
			const std::shared_ptr<Sampler> &sampler, int maxDepth)
			: Integrator(camera, sampler), maxDepth(maxDepth) {}
		virtual void preprocess(const Scene &scene, Sampler &sampler) override;
		virtual Spectrum Li(const Ray &ray, const std::shared_ptr<Scene> &scene,
			const std::shared_ptr<Sampler> &sampler, int depth) const override;

	private:
		// Generate path from camera & light
		// Because of ray come from both direction, wi or wo in this function may use inversed.
		// Specify direction of incident is wi 
		int generatePath(const Ray &ray, const Scene &scene, Sampler &sampler, Float pdf, std::vector<Vertex> &path) const;

		Float weightPath(const std::vector<Vertex> &cameraPath, int cameraPathSize,
						 const std::vector<Vertex> &lightPath, int lightPathSize,
						 const Vertex &lightVertex) const;
		Float pdfPath(const std::vector<Vertex> &cameraPath, int cameraPathSize,
					  const std::vector<Vertex> &lightPath, int lightPathSize,
					  const Vertex &lightVertex) const;
		Spectrum evaluatePath(const Scene &scene, const std::vector<Vertex> &cameraPath, int cameraPathSize,
							  const std::vector<Vertex> &lightPath, int lightPathSize) const;

		Float G(const Scene &scene, const Vertex &v0, const Vertex &p1) const;
	};

	std::shared_ptr<BDPathTracing> createBDPathTracingIntegrator(const std::shared_ptr<Camera> &camera,
		const std::shared_ptr<Sampler> &sampler, const ParamSet & param);
}

#endif // !ORION_INTEGRATOR_BDPT_H_
