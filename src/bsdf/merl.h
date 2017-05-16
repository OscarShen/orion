/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_BSDF_MERL_H_
#define ORION_BSDF_MERL_H_
#include <orion.h>
#include "bxdf.h"
namespace orion {

	constexpr int MERL_SAMPLING_RES_THETA_H = 90;
	constexpr int MERL_SAMPLING_RES_THETA_D = 90;
	constexpr int MERL_SAMPLING_RES_PHI_D = 180;
	constexpr int MERL_SAMPLING_COUNT = 1458000;
	constexpr double MERL_RED_SCALE = 0.0006666666666667;
	constexpr double MERL_GREEN_SCALE = 0.000766666666666667;
	constexpr double MERL_BLUE_SCALE = 0.0011066666666666667;

	class Merl : public BxDF
	{
	private:
		std::vector<Float> data;

	public:
		Merl() : BxDF((BxDF_TYPE)(BxDF_REFLECTION | BxDF_GLOSSY)) { }

		virtual Spectrum f(const Vector3f &swi, const Vector3f &swo) const override;

		virtual Spectrum sample_f(Vector3f *wi, const Vector3f &wo, Float *pdf) const override;

		std::shared_ptr<Merl> clone() const;

		static std::map<std::string, std::shared_ptr<Merl>> loadedMerl;

		friend std::shared_ptr<Merl> loadMerl(const std::string & filename);
	};

	std::shared_ptr<Merl> createMerl(const ParamSet &param);
}

#endif // !ORION_BSDF_MERL_H_
