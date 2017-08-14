/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_MATERIAL_KDSUBSURFACE_H_
#define ORION_MATERIAL_KDSUBSURFACE_H_

#include <core/material.h>
#include <core/bssrdf.h>
ORION_NAMESPACE_BEGIN

class KdSubsurface : public Material
{
private:
	Float scale;
	std::shared_ptr<Texture> Kd, Kr, Kt, mfp;
	std::shared_ptr<FloatTexture> roughness;
	Float eta;
	BSSRDFTable table;

public:
	KdSubsurface(Float scale,
		const std::shared_ptr<Texture> &Kd,
		const std::shared_ptr<Texture> &Kr,
		const std::shared_ptr<Texture> &Kt,
		const std::shared_ptr<Texture> &mfp,
		Float g, Float eta,
		const std::shared_ptr<FloatTexture> &roughness)
		:scale(scale), Kd(Kd), Kr(Kr), Kt(Kt), mfp(mfp),
		roughness(roughness), eta(eta), table(100, 64) {
		computeBeamDiffusionBSSRDF(g, eta, &table);
	}
	std::shared_ptr<BSDF> getBSDF(const Intersection &isec, TransportMode mode) const override;
	std::shared_ptr<BSSRDF> getBSSRDF(const Intersection &isec, TransportMode mode) const override;
};

std::shared_ptr<KdSubsurface> createKdSubsurfaceMaterial(std::shared_ptr<Texture> &Kd,
	std::shared_ptr<Texture> &mfp,
	std::shared_ptr<Texture> &Kr,
	std::shared_ptr<Texture> &Kt,
	std::shared_ptr<FloatTexture> &roughness,
	const ParamSet &param);

ORION_NAMESPACE_END

#endif // !ORION_MATERIAL_KDSUBSURFACE_H_
