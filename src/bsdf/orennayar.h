/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_BSDF_NAYAR_H_
#define ORION_BSDF_NAYAR_H_
#include <orion.h>
#include "bxdf.h"
namespace orion {

	class OrenNayar : public BxDF
	{
	private:
		const Spectrum R;
		Float A, B;

	public:
		OrenNayar(const Spectrum &R, Float sigma);

		virtual Spectrum f(const Vector3f &swi, const Vector3f &swo) const override;
	};

}

#endif // !ORION_BSDF_NAYAR_H_
