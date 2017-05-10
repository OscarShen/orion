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

	class Sampler
	{
	private:
		int cursor = 0;
		long long instance;

	public:
		Sampler() : instance(74207281LL) {}
		Sampler(long long instance) : instance(instance) {}
		virtual Float sample(int d, long long i) = 0;

		Float next() { return sample(cursor++, instance); }
		Point2f next2() { return Point2f(next(), next()); }
		Point3f next3() { return Point3f(next(), next(), next()); }
	};

	class PseudoRandomSampler : public Sampler
	{
	public:
		virtual Float sample(int d, long long i) override {
			return orion::rand();
		}
	};

	std::shared_ptr<PseudoRandomSampler> createPseudoSampler();
}

#endif // !ORION_SAMPLER_H_
