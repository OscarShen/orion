/************************************************************************ 
 * @description :  mirror material
 * @author		:  Oscar Shen
 * @creat 		:  2017-7-5 14:29:08
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_MATERIAL_MIRROR_H_
#define ORION_MATERIAL_MIRROR_H_

#include <core/material.h>
ORION_NAMESPACE_BEGIN

class Mirror : public Material
{
private:
	std::shared_ptr<Texture> Kr;

public:
	Mirror(const std::shared_ptr<Texture> &Kr) : Kr(Kr) {}
	std::shared_ptr<BSDF> getBSDF(const Intersection &isec, TransportMode mode) const override;
};

std::shared_ptr<Mirror> createMirrorMaterial(const std::shared_ptr<Texture> &Kr);

ORION_NAMESPACE_END

#endif // !ORION_MATERIAL_MIRROR_H_
