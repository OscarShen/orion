/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_SCENE_H_
#define ORION_SCENE_H_
#include <orion.h>
ORION_NAMESPACE_BEGIN

class Scene {

	std::shared_ptr<Sampler> sampler;
	std::shared_ptr<Camera> camera;

};

ORION_NAMESPACE_END


#endif // !ORION_SCENE_H_
