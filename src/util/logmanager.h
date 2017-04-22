/************************************************************************ 
 * @description :  log manager, 由于现在仍然是一个小系统，暂时不使用日志功能，
				   随后修改 CHECK 宏可以快速应用。
 * @author		:  Oscar Shen
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_LOG_MANAGER_H_
#define ORION_LOG_MANAGER_H_
#include <orion.h>
#include <common/singleton.h>
#include <util/timer.h>
namespace orion {

#define	LOG_FILE_INFO	"(File:"<<__FILE__<<"  Line:"<<__LINE__<<") "
#define LOG				LogManager::inst()
#define LOG_WARNING		LOG<<LOG_FILE_INFO<<"WARNING: "
#define	LOG_ERROR		LOG<<LOG_FILE_INFO<<"ERROR: "

#define ENDL _ENDL(false)
#define	CRASH _ENDL(true)

	class _ENDL
	{
	public:
		bool crash;

		_ENDL(bool crash) { this->crash = crash; }
	};

	class LogManager : public Singleton<LogManager>
	{
	private:
		std::ofstream ofs;
		std::string logPath;
		bool logEnabled;

	public:
		static void init() { new LogManager(); }
		~LogManager() { ofs.close(); }
		void setLog(bool enable);

	private:
		LogManager() : logPath("log.txt"), logEnabled(true) { _init(); }
		void _init();

		friend LogManager &operator<<(LogManager &lm, int i);
		friend LogManager& operator<<(LogManager &lm, Float f);
		friend LogManager& operator<<(LogManager &lm, const char* str);
		friend LogManager& operator<<(LogManager &lm, const std::string& str);
		friend LogManager& operator<<(LogManager &lm, _ENDL endl);
	};
}

#endif // !ORION_LOG_MANAGER_H_
