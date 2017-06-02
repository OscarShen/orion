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
	private:
		int cursor = 0;
		long long instance;

	public:
		Sampler() : instance(74207281LL) {}
		Sampler(long long instance) : instance(instance) {}
		virtual Float sample(int d, long long i) = 0;
		virtual ~Sampler() {}

		Float next() { 
			Float s;
#pragma omp critical
			{
				s = sample(cursor++, instance);
			}
			return s;
		}
		Point2f next2() { return Point2f(next(), next()); }
		Point3f next3() { return Point3f(next(), next(), next()); }
		virtual std::shared_ptr<Sampler> clone(int seed) = 0;
	};

	class PseudoRandomSampler : public Sampler
	{
	public:
		virtual Float sample(int d, long long i) override {
			return orion::rand();
		}

		virtual std::shared_ptr<Sampler> clone(int seed) override {
			return std::make_shared<PseudoRandomSampler>();
		}
	};

	std::shared_ptr<PseudoRandomSampler> createPseudoSampler();
}

#endif // !ORION_SAMPLER_H_
