#include "envvariable.h"

namespace orion {

	std::string getExecutiveDir()
	{
		const int maxLen = 2048;
		char buf[maxLen];

	#if defined(ORION_IN_LINUX)
		int c = readlink("/proc/self/exe", buf, maxLen - 1);
	#elif defined(ORION_IN_WINDOWS)
		// get the current module filename
		GetModuleFileNameA(nullptr, buf, maxLen);

		// remove the file name
		int len = (int)strlen(buf);
		for (int i = len - 1; i > 0; i--)
		{
			if (buf[i] == '\\')
			{
				buf[i + 1] = 0;
				break;
			}
		}
	#endif
		return std::string(buf);
	}

	static std::string	RES_PATH = "";

	std::string getFullPath(const std::string & name)
	{
		return getExecutiveDir() + name;
	}

	void setResPath(const std::string & path)
	{
		RES_PATH = getExecutiveDir() + path;
	}

	std::string getResPath()
	{
		return RES_PATH;
	}

}
