#include "sampler.h"
#include <common/paramset.h>
namespace orion {

	std::shared_ptr<PseudoRandomSampler> createPseudoSampler()
	{
		return std::shared_ptr<PseudoRandomSampler>(new PseudoRandomSampler());
	}

}
