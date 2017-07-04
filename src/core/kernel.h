/************************************************************************ 
 * @description :  kernel of intersection
 * @author		:  Oscar Shen
 * @creat 		:  2017-6-27 16:38:38
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_CORE_KERNEL_H_
#define ORION_CORE_KERNEL_H_
#include <orion.h>
#include "geometry.h"
#include <embree2/rtcore.h>
ORION_NAMESPACE_BEGIN

class Kernel
{
public:
	Kernel() {}
	virtual ~Kernel() {}
	virtual void clear() = 0;
	virtual void build() = 0;
	virtual bool intersect(const Ray &ray, Intersection *isec) const = 0;
	virtual Bounds3f worldBound() const = 0;
};

class EmbreeKernel : public Kernel
{
private:
	std::vector<std::shared_ptr<Primitive>> primitives;
	RTCDevice rtcDevice;
	RTCScene rtcScene;
	int geomID;
	Bounds3f bound;

public:
	EmbreeKernel(const std::vector<std::shared_ptr<Primitive>> &primitives);
	~EmbreeKernel();
	void clear() override;
	void build() override;
	bool intersect(const Ray &ray, Intersection *isec) const override;
	Bounds3f worldBound() const override;
};

ORION_NAMESPACE_END

#endif // !ORION_CORE_KERNEL_H_
