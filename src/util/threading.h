/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_UTIL_THREADING_H_
#define ORION_UTIL_THREADING_H_

#include <orion.h>
#include <core/geometry.h>
ORION_NAMESPACE_BEGIN

bool multiThreadEnabled();

void setMultiThread(bool enable);

int numSystemCores();

int threadID();

class Spinlock
{
protected:
	std::atomic<bool> latch;

public:
	Spinlock() { latch.store(false); }
	Spinlock(bool flag) { latch.store(flag); }
	Spinlock(int flag) { latch.store(flag != 0); }

	Spinlock(const Spinlock &) = delete;
	Spinlock &operator=(const Spinlock &) = delete;

	void lock() {
		bool unlatched = false;
		while (!latch.compare_exchange_weak(unlatched, true, std::memory_order_acquire)) {
			unlatched = false;
		}
	}

	void unlock() {
		latch.store(false, std::memory_order_release);
	}
};

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

ORION_NAMESPACE_END

#endif // !ORION_UTIL_THREADING_H_
