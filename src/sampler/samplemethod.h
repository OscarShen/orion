/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_SAMPLE_METHOD_H_
#define ORION_SAMPLE_METHOD_H_
#include <orion.h>
#include <core/geometry.h>
namespace orion {

	Point2f	uniformSampleDisk(const Point2f &u);

	Point2f uniformSampleTriangle(const Point2f &u);

}

#endif // !ORION_SAMPLE_METHOD_H_
