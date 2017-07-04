/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_MATERIAL_UBER_H_
#define ORION_MATERIAL_UBER_H_
#include <orion.h>
#include <material/material.h>
namespace orion {

	class UberMaterial : public Material {
	public:
		UberMaterial(const std::shared_ptr<Texture> &Kd,
			const std::shared_ptr<Texture> &Ks,
			const std::shared_ptr<Texture> &Kr,
			const std::shared_ptr<Texture> &Kt,
			const std::shared_ptr<Texture> &opacity,
			const std::shared_ptr<FloatTexture> &roughness,
			const std::shared_ptr<FloatTexture> &eta)
			: Kd(Kd),
			Ks(Ks),
			Kr(Kr),
			Kt(Kt),
			opacity(opacity),
			roughness(roughness),
			eta(eta),
			bumpMap(bumpMap) {}

		virtual std::shared_ptr<BSDF> getBSDF(const Intersection *isec, bool hasEtaAtenuation = true) const override;

	private:
		// UberMaterial Private Data
		std::shared_ptr<Texture> Kd, Ks, Kr, Kt, opacity;
		std::shared_ptr<FloatTexture> roughness, eta, bumpMap;
	};

	std::shared_ptr<UberMaterial> createUberMaterial(const std::shared_ptr<Texture> &Kd,
		const std::shared_ptr<Texture> &Ks,
		const std::shared_ptr<Texture> &Kr,
		const std::shared_ptr<Texture> &Kt,
		const std::shared_ptr<Texture> &opacity,
		const std::shared_ptr<FloatTexture> &roughness,
		const std::shared_ptr<FloatTexture> &eta);
}

#endif // !ORION_MATERIAL_UBER_H_
