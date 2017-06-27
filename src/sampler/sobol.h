/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_SAMPLER_SOBOL_H_
#define ORION_SAMPLER_SOBOL_H_
#include <orion.h>
#include <core/sampler.h>

namespace sobol {

	struct Matrices
	{
		static const unsigned num_dimensions = 1024;
		static const unsigned size = 52;
		static const unsigned matrices[];
	};

	// Compute one component of the Sobol'-sequence, where the component
	// corresponds to the dimension parameter, and the index specifies
	// the point inside the sequence. The scramble parameter can be used
	// to permute elementary intervals, and might be chosen randomly to
	// generate a randomized QMC sequence.
	inline float sample(
		unsigned long long index,
		const unsigned dimension,
		const unsigned scramble = 0U)
	{
		CHECK(dimension < Matrices::num_dimensions);

		unsigned result = scramble;
		for (unsigned i = dimension * Matrices::size; index; index >>= 1, ++i)
		{
			if (index & 1)
				result ^= Matrices::matrices[i];
		}

		return result * (1.f / (1ULL << 32));
	}
}

namespace orion {

	class SobolSampler : public Sampler
	{
	public:
		SobolSampler(int64_t index) : Sampler(index) {}
		SobolSampler(const SobolSampler &sampler) : SobolSampler(sampler.index + 1) {}
		virtual Float sample() override {
			return sobol::sample(index, dimention++);
		}

		virtual std::unique_ptr<Sampler> clone(int64_t seed) override {
			return std::unique_ptr<SobolSampler>(new SobolSampler(seed));
		}
	};

	std::shared_ptr<SobolSampler> createSobolSampler();
}

#endif // !ORION_SAMPLER_SOBOL_H_
