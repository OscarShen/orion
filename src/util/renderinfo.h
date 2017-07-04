/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_UTIL_RENDER_INFO_H_
#define ORION_UTIL_RENDER_INFO_H_

#include <orion.h>
#include "timer.h"
#include "threading.h"
ORION_NAMESPACE_BEGIN

class ProcessReporter
{
private:
	std::atomic<uint64_t> workDone;
	uint64_t totalWork;
	Timer timer;
	Spinlock lock;

public:
	ProcessReporter(uint64_t totalWork) : totalWork(totalWork) {}

	void done() {
		++workDone;
		outputProgress();
	}

private:
	void outputProgress();
};

ORION_NAMESPACE_END

#endif // !ORION_UTIL_RENDER_INFO+H_
