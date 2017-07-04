#include "disk.h"
#include <common/paramset.h>
#include <util/strutil.h>
#include <sampler/sampling.h>
namespace orion {
	bool Disk::intersect(const Ray & ray, Intersection * isec) const
	{
		Ray r = (*world2local)(ray);

		// No parallel
		if (r.d.y == 0) return false;
		Float tHit = -r.o.y / r.d.y;
		if (tHit <= 0 || tHit >= r.tMax) return false;

		Point3f pHit = r(tHit);
		Float dist2 = pHit.x * pHit.x + pHit.z * pHit.z;
		if (dist2 > radius * radius) return false; // No intersection

		Float phi = std::atan2(-pHit.z, pHit.x);
		if (phi < 0) phi += 2 * pi;
		
		Float u = phi / (2 * pi);
		Float rHit = std::sqrt(dist2);
		Float v = rHit / radius;

		Vector3f dpdu(2 * pi * rHit * pHit.z, 0, -2 * pi * rHit * pHit.x); // You should calculate by youself, this is the case for
																		   // right hand coordination and y for up!
		Vector3f dpdv(rHit * pHit.x, 0, rHit * pHit.y);

		*isec = (*local2world)(Intersection(pHit, Normal3f(0.0f, 1.0f, 0.0f), Point2f(u, v), tHit, dpdu, dpdv));
		return true;
	}
	Bounds3f Disk::localBound() const
	{
		return Bounds3f(Point3f(-radius, 0, -radius),
						Point3f(radius, 0, radius));
	}
	Float Disk::area() const
	{
		return pi * radius * radius;
	}
	Intersection Disk::sample(const Point2f & u, Float * pdf) const
	{
		Point2f sampling = uniformSampleDisk(u);
		Point3f pos(sampling.x * radius, 0, sampling.y * radius);
		Intersection isec;
		isec.n = normalize((*local2world)(Normal3f(0, 1, 0)));
		isec.pHit = (*local2world)(pos);
		*pdf = 1 / area();
		return isec;
	}
	std::shared_ptr<Disk> createDisk(const Transform * local2world, const Transform * world2local, const ParamSet & param)
	{
		Float radius = parseFloat(param.getParam("radius"));
		return std::shared_ptr<Disk>(new Disk(local2world, world2local, radius));
	}
}
