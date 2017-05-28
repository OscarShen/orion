#include "sphere.h"
#include <sampler/sampling.h>
#include <util/strutil.h>
namespace orion {
	bool Sphere::intersect(const Ray & ray, Intersection * isec) const
	{
		Float phi;
		Point3f pHit;
		Ray r = (*world2local)(ray);

		Float ox = r.o.x, oy = r.o.y, oz = r.o.z;
		Float dx = r.d.x, dy = r.d.y, dz = r.d.z;
		Float a = dx * dx + dy * dy + dz * dz;
		Float b = 2 * (dx * ox + dy * oy + dz * oz);
		Float c = ox * ox + oy * oy + oz * oz - radius * radius;

		// solved quadratic equation
		Float t0, t1;
		if (!quadratic(a, b, c, &t0, &t1)) return false;

		if (t0 > r.tMax || t1 <= 0) return false;
		Float tHit = t0;
		if (tHit <= 0) {
			tHit = t1;
			if (tHit > r.tMax) return false;
		}

		pHit = r(tHit);
		pHit *= radius / (pHit - Point3f(0)).length(); // refine pHit
		if (pHit.x == 0 && pHit.z == 0) pHit.x = 1e-5f * radius;
		phi = std::atan2(-pHit.z, pHit.x);
		if (phi < 0) phi += 2 * pi;

		Float u = phi / (2 * pi);
		Float theta = std::acos(clamp(pHit.y / radius, -1.0f, 1.0f));
		Float v = theta / pi;

		Float yRadius = std::sqrt(pHit.x * pHit.x + pHit.z * pHit.z);
		Float invYRadius = 1 / yRadius;
		Float cosPhi = pHit.x * invYRadius;
		Float sinPhi = -pHit.z * invYRadius;
		Vector3f dpdu(2 * pi * pHit.z, 0, -2 * pi * pHit.x);
		Vector3f dpdv = pi * Vector3f(cosPhi * pHit.y, -radius * std::sin(theta), -pHit.y * sinPhi);

		Normal3f n(pHit.x, pHit.y, pHit.z);
		if (Vector3f(r.o).length() < radius) n = -n;

		*isec = (*local2world)(Intersection(pHit, n, Point2f(u, v), r.time, dpdu, dpdv));
		return true;
	}
	Bounds3f Sphere::localBound() const
	{
		return Bounds3f(Point3f(-radius), Point3f(radius));
	}
	Float Sphere::area() const
	{
		return 4 * pi * radius * radius;
	}
	Intersection Sphere::sample(const Point2f & rand, Float * pdf) const
	{
		Point3f pObj = Point3f(0) + radius * uniformSampleSphere(rand);
		Intersection isec;
		isec.n = normalize((*local2world)(Normal3f(pObj.x, pObj.y, pObj.z)));
		pObj *= radius / (pObj - Point3f(0)).length();
		isec.pHit = (*local2world)(pObj);
		*pdf = 1 / area();
		return isec;
	}
	Intersection Sphere::sample(const Intersection & isec, const Point2f & rand, Float * pdf) const
	{
		Point3f pCenter = (*local2world)(Point3f(0));
		Point3f pOrigin = isec.pHit + epsilon * (pCenter - isec.pHit);

		if ((pOrigin - pCenter).lengthSquared() <= radius * radius) {
			Intersection in = sample(rand, pdf);
			in.n = -in.n;
			Vector3f wi = in.pHit - isec.pHit;
			if (wi.lengthSquared() == 0) {
				*pdf = 0;
			}
			else {
				wi = normalize(wi);
				*pdf *= (isec.pHit - in.pHit).lengthSquared() / absDot(in.n, -wi);
			}
			if (std::isinf(*pdf)) *pdf = 0;
			return in;
		}

		Vector3f wcY = normalize(pCenter - isec.pHit);
		Vector3f wcX, wcZ;
		coordinateSystem(wcY, &wcZ, &wcX);
		
		Float sinThetaMax2 = radius * radius / (isec.pHit - pCenter).lengthSquared();
		Float cosThetaMax = std::sqrt(std::max((Float)0, 1 - sinThetaMax2));
		Float cosTheta = (1 - rand[0]) + rand[0] * cosThetaMax;
		Float sinTheta = std::sqrt(std::max((Float)0, 1 - cosTheta * cosTheta));
		Float phi = rand[1] * 2 * pi;

		// Compute angle $\alpha$ from center of sphere to sampled point on surface
		Float dc = (isec.pHit - pCenter).length();
		Float ds = dc * cosTheta -
			std::sqrt(std::max(
			(Float)0, radius * radius - dc * dc * sinTheta * sinTheta));
		Float cosAlpha = (dc * dc + radius * radius - ds * ds) / (2 * dc * radius);
		Float sinAlpha = std::sqrt(std::max((Float)0, 1 - cosAlpha * cosAlpha));

		Vector3f nWorld = sphericalDirection(sinAlpha, cosAlpha, phi, -wcX, -wcY, -wcZ);
		Point3f pWorld = pCenter + radius * Point3f(nWorld.x, nWorld.y, nWorld.z);

		Intersection in;
		in.pHit = pWorld;
		in.n = Normal3f(nWorld);

		*pdf = 1 / (2 * pi *(1 - cosThetaMax));
		return in;
	}
	Float Sphere::pdf(const Intersection & isec, const Vector3f & wi) const
	{
		Point3f pCenter = (*local2world)(Point3f(0, 0, 0));
		// Return uniform PDF if point is inside sphere
		Point3f pOrigin = isec.pHit + epsilon * (pCenter - isec.pHit);

		if ((pOrigin - pCenter).lengthSquared() <= radius * radius)
			return Shape::pdf(isec, wi);

		// Compute general sphere PDF
		Float sinThetaMax2 = radius * radius / (isec.pHit - pCenter).lengthSquared();
		Float cosThetaMax = std::sqrt(std::max((Float)0, 1 - sinThetaMax2));
		return uniformConePdf(cosThetaMax);
	}
	std::shared_ptr<Sphere> createSphere(const Transform * local2world, const Transform * world2local, const ParamSet & param)
	{
		Float radius = parseFloat(param.getParam("radius"));
		return std::shared_ptr<Sphere>(new Sphere(local2world, world2local, radius));
	}
}
