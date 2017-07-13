/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_SAMPLER_PSEUDO_H_
#define ORION_SAMPLER_PSEUDO_H_
#include <orion.h>
#include <core/sampler.h>
ORION_NAMESPACE_BEGIN

class PseudoRandomSampler : public Sampler
{
private:
	std::default_random_engine e;
	std::uniform_real_distribution<Float> u;

public:
	PseudoRandomSampler(int64_t seed = 5) {
		e.seed(seed);
	}

	Float sample() override {
		return u(e);
	}

	std::unique_ptr<Sampler> clone(int64_t seed) override {
		return std::make_unique<PseudoRandomSampler>(seed);
	}
};

std::shared_ptr<PseudoRandomSampler> createPseudoSampler() {
	return std::make_shared<PseudoRandomSampler>();
}

ORION_NAMESPACE_END

#endif // !ORION_SAMPLER_PSEUDO_H_
