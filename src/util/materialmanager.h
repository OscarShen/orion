/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_UTIL_MATERIAL_MANAGER_H_
#define ORION_UTIL_MATERIAL_MANAGER_H_
#include <orion.h>
#include <common/singleton.h>
namespace orion {

	class MaterialManager : public Singleton<MaterialManager>
	{
	private:
		std::map<std::string, std::shared_ptr<Material>> materials;

	public:
		static void init() { new MaterialManager(); }
		void addMaterial(const std::string &name, const std::shared_ptr<Material> &material);
		std::shared_ptr<Material> getMaterial(const std::string &name);

	private:
		MaterialManager() {}
	};
}

#endif // !ORION_UTIL_MATERIAL_MANAGER_H_
