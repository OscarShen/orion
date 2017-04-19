/************************************************************************ 
 * @description :  singleton
 * @author		:  Oscar Shen
 * @creat 		:  2017-4-19 10:45:27
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_SINGLETON_H_
#define ORION_SINGLETON_H_
#include <orion.h>
namespace orion {

	template <typename T>
	class Singleton
	{
	private:
		static std::shared_ptr<T> _instance;

	public:
		static std::shared_ptr<T> inst()
		{
			CHECK_INFO_EXIT(_instance.get() != nullptr, "Please initialize before using!");
			return _instance;
		}
		static void _delete() {
			_instance.reset();
		}

		virtual ~Singleton() {}

	protected:
		Singleton() {
			_instance = std::shared_ptr<T>(reinterpret_cast<T*>(this)); }

	private:
		Singleton(const Singleton&);
		Singleton& operator=(const Singleton&) {}
	};
}

#endif // !ORION_SINGLETON_H_
