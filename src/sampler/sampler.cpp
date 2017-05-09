#include "sampler.h"
#include "pseudo.h"
#include <common/paramset.h>
namespace orion {

	Float RandomSequence::sample()
	{
		CHECK_INFO(sampler != nullptr, "No sampler!");
		Float ret = sampler->sample(cursor++, instance);
		CHECK_INFO(ret >= 0, "Sampler output should be non-neg!");
		if (ret > 1 + 1e-5f) {
			printf("Warning: sampler returns value > 1: [%f]", ret);
		}
		if (ret >= 1) {
			ret = 0;
		}
		return ret;
	}
}
