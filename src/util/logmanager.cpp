#include "logmanager.h"

namespace orion {

	DEFINE_SINGLETON(LogManager);

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

}
