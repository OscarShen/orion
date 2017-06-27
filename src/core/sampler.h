/************************************************************************
* @description :  sampler
* @author		:  Oscar Shen
* @creat 		:  2017Äê5ÔÂ10ÈÕ20:01:05
************************************************************************
* Copyright @ OscarShen 2017. All rights reserved.
************************************************************************/
#pragma once
#ifndef ORION_CORE_SAMPLER_H_
#define ORION_CORE_SAMPLER_H_
#include <orion.h>
#include "geometry.h"
ORION_NAMESPACE_BEGIN

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
		return sample();
	}
	Point2f next2() { return Point2f(next(), next()); }
	Point3f next3() { return Point3f(next(), next(), next()); }
	virtual std::unique_ptr<Sampler> clone(int64_t seed) = 0;
};

ORION_NAMESPACE_END
#endif // !ORION_CORE_SAMPLER_H_
