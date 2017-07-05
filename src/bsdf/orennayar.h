/************************************************************************ 
 * @description :  a more realistic diffuse model than lambert
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_BSDF_OREN_NAYAR_H_
#define ORION_BSDF_OREN_NAYAR_H_

#include <core/bsdf.h>
ORION_NAMESPACE_BEGIN

class OrenNayar : public BxDF
{
private:
	Float A, B; // two param of orennayar scattering function
	Spectrum R;

public:
	OrenNayar(const Spectrum &R, Float sigma);

	Spectrum f(const Vector3f &swi, const Vector3f &swo) const override;
};

ORION_NAMESPACE_END

#endif // !ORION_BSDF_OREN_NAYAR_H_
