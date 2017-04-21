/************************************************************************ 
 * @description :  timer in millisecond resolution
 * @author		:  Oscar Shen
 * @creat 		:  2017-4-21 17:46:254
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_TIMER_H_
#define ORION_TIMER_H_
#include <orion.h>
#include <common/singleton.h>
#include <chrono>
namespace orion {

	class Timer : public Singleton<Timer>
	{
		typedef std::chrono::duration<uint64_t, std::milli> milli_second_duration;
		typedef std::chrono::time_point<std::chrono::system_clock, milli_second_duration> milli_second_point;
	private:
		milli_second_point stamp;

	public:
		static void init() { new Timer(); }

		void reset() {
			stamp = std::chrono::time_point_cast<milli_second_duration>(std::chrono::system_clock::now());
		}

		// Get elapsed time from last _reset()_, the first _reset()_ is called when you init timer
		// return _uint64_t_ of milliseconds
		uint64_t getElaspedTime() const {
			milli_second_point now = std::chrono::time_point_cast<milli_second_duration>(std::chrono::system_clock::now());
			return milli_second_duration(now - stamp).count();
		}

	private:
		Timer() {
			reset();
		}
	};

}

#endif // !ORION_TIMER_H_
