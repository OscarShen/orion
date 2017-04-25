/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_ACCELERATOR_H_
#define ORION_ACCELERATOR_H_
#include <orion.h>
#include <core/geometry.h>
#include <shape/shape.h>
namespace orion {

	class Accelerator
	{
	public:
		Accelerator() {}
		~Accelerator() {}

		virtual bool intersect(const Ray &ray, Intersection *isec) const = 0;
	};

}

#endif // !ORION_ACCELERATOR_H_
