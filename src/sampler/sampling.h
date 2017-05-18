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

	Point2f	uniformSampleDisk(const Point2f &rand);
	Point2f concentricSampleDisk(const Point2f &rand);

	Point2f uniformSampleTriangle(const Point2f &rand);

	inline Vector3f cosineSampleHemisphere(const Point2f &rand) {
		Point2f sample = concentricSampleDisk(rand);
		Float y = std::sqrt(std::max(0.0f, 1 - sample[0] * sample[0] - sample[1] * sample[1]));
		return Vector3f(sample[0], y, sample[1]);
	}

	inline Float powerHeuristic(int nf, Float fPdf, int ng, Float gPdf) {
		Float f = nf * fPdf, g = ng * gPdf;
		return (f * f) / (f * f + g * g);
	}

}

#endif // !ORION_SAMPLE_METHOD_H_
