/************************************************************************ 
 * @description :  sampler
 * @author		:  Oscar Shen
 * @creat 		:  2017Äê5ÔÂ10ÈÕ20:01:05
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_SAMPLER_H_
#define ORION_SAMPLER_H_
#include <orion.h>
#include <core/geometry.h>
#include <math/mathutil.h>
namespace orion {

	constexpr Float oneMinusEpsilon = sizeof(Float) == sizeof(float) ? 0.99999994f : 0.99999999999999989;

	class Sampler
	{
	protected:
		int64_t index;
		int dimention;

	public:
		Sampler() {}
		Sampler(int64_t index) : index(index), dimention(5) {}
		Sampler(const Sampler &sampler) : index(sampler.index + 1), dimention(5) {}
		virtual Float sample() = 0;
		virtual ~Sampler() {}

		Float next() {
			Float f;
#pragma omp critical
			{
				f = sample();
			}
			return f;
		}
		Point2f next2() { return Point2f(next(), next()); }
		Point3f next3() { return Point3f(next(), next(), next()); }
		virtual std::unique_ptr<Sampler> clone(int64_t seed) = 0;
	};

	class PseudoRandomSampler : public Sampler
	{
	public:
		virtual Float sample() override {
			return orion::rand();
		}

		virtual std::unique_ptr<Sampler> clone(int64_t seed) override {
			return std::make_unique<PseudoRandomSampler>();
		}
	};

	std::shared_ptr<PseudoRandomSampler> createPseudoSampler();
}

#endif // !ORION_SAMPLER_H_
