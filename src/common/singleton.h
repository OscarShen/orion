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
			CHECK_INFO(_instance.get() != nullptr, "Not initialized before use");
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

#define DEFINE_SINGLETON(T) template <> std::shared_ptr<T> Singleton<T>::_instance = nullptr;
}

#endif // !ORION_SINGLETON_H_
