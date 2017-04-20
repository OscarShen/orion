/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_LOG_MANAGER_H_
#define ORION_LOG_MANAGER_H_
#include <orion.h>
#include <common/singleton.h>
namespace orion {
	class LogManager : public Singleton<LogManager>
	{
	private:
		std::ofstream ofs;
		std::string logPath;
		bool enabled;

	public:
		static void init() { new LogManager(); }
		~LogManager() { ofs.close(); }
		void enableLog(bool b) { enabled = b; }
		void log(const std::string &log, bool enter = true) {
			if (enabled) {
				ofs << log;
				if (enter)
					ofs << std::endl;
			}
		}

	private:
		LogManager() : logPath("log.txt"), enabled(true) { _init(); }
		void _init();
	};
}

#endif // !ORION_LOG_MANAGER_H_
