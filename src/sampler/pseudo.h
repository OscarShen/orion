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
#include <math/mathutil.h>
#include "sampler.h"
namespace orion {

	class PseudoRandomSampler : public Sampler
	{
	public:
		Float sample(int d, long long i) const {
			return rand();
		}
	};

	std::shared_ptr<PseudoRandomSampler> createPseudoSampler();
}

#endif // !ORION_SAMPLER_PSEUDO_H_
