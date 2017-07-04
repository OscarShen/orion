#include "threading.h"
#ifdef ORION_IN_WINDOWS
#include <Windows.h>
#endif
#include <omp.h>

ORION_NAMESPACE_BEGIN
// default enabled
static bool isMultiThreadEnabled = true;

bool multiThreadEnabled()
{
	return isMultiThreadEnabled;
}
void setMultiThread(bool enable)
{
	isMultiThreadEnabled = enable;
}

int numSystemCores()
{
	if (isMultiThreadEnabled)
	{
#ifdef ORION_IN_WINDOWS
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		return sysinfo.dwNumberOfProcessors;
#elif defined(SORT_IN_LINUX)
		return sysconf(_SC_NPROCESSORS_ONLN);
#endif
	}
	return 1;
}

int threadID()
{
	if (isMultiThreadEnabled)
		return omp_get_thread_num();
	return 0;
}
ORION_NAMESPACE_END