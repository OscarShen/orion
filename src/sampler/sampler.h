/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_SAMPLER_H_
#define ORION_SAMPLER_H_
#include <orion.h>
#include <core/geometry.h>
namespace orion {

	class StateSequence
	{
	protected:
		int cursor = 0;

	public:
		virtual Float sample() = 0;
		virtual Float operator()() {
			return sample();
		}
		int getCursor() const { return cursor; }
		void checkCursorPos(int cursor) const {
			CHECK_INFO(this->cursor == cursor, std::string("Cursor position should be " + std::to_string(cursor) +
				" instead of " + std::to_string(this->cursor)));
		}
		Vector2f next2() { return Vector2f(sample(), sample()); }
		Vector3f next3() { return Vector3f(sample(), sample(), sample()); }
	};

	class Sampler
	{
	public:
		virtual Float sample(int d, long long i) const = 0;
	};

	class RandomSequence : public StateSequence
	{
	private:
		std::shared_ptr<Sampler> sampler;
		long long instance;

	public:
		RandomSequence(const std::shared_ptr<Sampler> &sampler, long long instance)
			: sampler(sampler), instance(instance) {}

		Float sample() override;
	};
}

#endif // !ORION_SAMPLER_H_
