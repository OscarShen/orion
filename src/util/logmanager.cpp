#include "logmanager.h"

namespace orion {

	DEFINE_SINGLETON(LogManager);

	void LogManager::setLog(bool enable)
	{
		if(enable==false && logEnabled == true)

		logEnabled = enable;

	}

	void LogManager::_init()
	{
		ofs.open(logPath);
		if (!ofs.is_open()) {
			std::cout << "Could not open log file!" << std::endl;
		}
		ofs << "-------------------------------------------------------------------------------------------------" << std::endl;
		ofs << "                                This is the log file for ORION." << std::endl;
		ofs << "-------------------------------------------------------------------------------------------------" << std::endl;
	}

	LogManager & operator<<(LogManager & lm, int i)
	{
		if (lm.logEnabled)
			lm.ofs << i;
		return lm;
	}

	LogManager & operator<<(LogManager & lm, Float f)
	{
		if (lm.logEnabled)
			lm.ofs << f;
		return lm;
	}

	LogManager & operator<<(LogManager & lm, const char * str)
	{
		if (lm.logEnabled)
			lm.ofs << str;
		return lm;
	}

	LogManager & operator<<(LogManager & lm, const std::string & str)
	{
		if (lm.logEnabled)
			lm.ofs << str;
		return lm;
	}

	LogManager & operator<<(LogManager & lm, _ENDL endl)
	{
		if (lm.logEnabled) {
			lm.ofs << " | " << Timer::getTimeStrNow() << std::endl;
		}
		if (endl.crash)
			abort();
		return lm;
	}

}
