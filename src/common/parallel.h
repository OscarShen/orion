/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_COMMON_PARALLEL_H_
#define ORION_COMMON_PARALLEL_H_
#include <orion.h>
namespace orion {

	bool multiThreadEnabled();

	void setMultiThread(bool enable);

	int numSystemCores();

	int threadID();

	inline uint32_t floatToBits(float f) {
		uint32_t ui;
		memcpy(&ui, &f, sizeof(float));
		return ui;
	}

	inline float bitsToFloat(uint32_t ui) {
		float f;
		memcpy(&f, &ui, sizeof(uint32_t));
		return f;
	}

	inline uint64_t floatToBits(double f) {
		uint64_t ui;
		memcpy(&ui, &f, sizeof(double));
		return ui;
	}

	inline double bitsToFloat(uint64_t ui) {
		double f;
		memcpy(&f, &ui, sizeof(uint64_t));
		return f;
	}

	class AtomicFloat
	{
	private:
#ifdef ORION_FLOAT_AS_DOUBLE
		std::atomic<uint64_t> bits;
#else
		std::atomic<uint32_t> bits;
#endif // ORION_FLOAT_AS_DOUBLE
	public:
		explicit AtomicFloat(Float v = 0) { bits = floatToBits(v); }
		operator Float() const { return bitsToFloat(bits); }
		Float operator=(Float v) { bits = floatToBits(v); return v; }
		void add(Float v) {
#ifdef ORION_FLOAT_AS_DOUBLE
			uint64_t oldBits = bits, newBits;
#else
			uint32_t oldBits = bits, newBits;
#endif
			do {
				newBits = floatToBits(bitsToFloat(oldBits) + v);
			} while (!bits.compare_exchange_weak(oldBits, newBits));
		}
	};
}
#endif // !ORION_COMMON_PARALLEL_H_
