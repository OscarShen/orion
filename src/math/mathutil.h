/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_MATH_UTIL_H_
#define ORION_MATH_UTIL_H_
namespace orion {
	
	inline float rand() {
			static uint64_t state = 0x853c49e6748fea9bULL, inc = 0xda3e39cb94b95bdbULL;
			uint64_t oldstate = state;
			state = oldstate * 0x5851f42d4c957f2dULL + inc;
			uint32_t xorshifted = (uint32_t)(((oldstate >> 18u) ^ oldstate) >> 27u);
			uint32_t rot = (uint32_t)(oldstate >> 59u);
			return ((xorshifted >> rot) | (xorshifted << ((~rot + 1u) & 31))) * 2.3283064365386963e-10f;
	}
}

#endif // !ORION_MATH_UTIL_H_
