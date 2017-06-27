#include "kernel.h"
#include "geometry.h"
#include "primitive.h"
#include "intersection.h"
#include <embree2/rtcore_ray.h>
ORION_NAMESPACE_BEGIN

/* error reporting function */
void errorHandler(const RTCError code, const char *str = nullptr) {
	if (code == RTC_NO_ERROR)
		return;

	printf("Embree: ");
	switch (code) {
	case RTC_UNKNOWN_ERROR:
		printf("RTC_UNKNOWN_ERROR");
		break;
	case RTC_INVALID_ARGUMENT:
		printf("RTC_INVALID_ARGUMENT");
		break;
	case RTC_INVALID_OPERATION:
		printf("RTC_INVALID_OPERATION");
		break;
	case RTC_OUT_OF_MEMORY:
		printf("RTC_OUT_OF_MEMORY");
		break;
	case RTC_UNSUPPORTED_CPU:
		printf("RTC_UNSUPPORTED_CPU");
		break;
	case RTC_CANCELLED:
		printf("RTC_CANCELLED");
		break;
	default:
		printf("invalid error code");
		break;
	}
	if (str) {
		printf(" (");
		while (*str) putchar(*str++);
		printf(")\n");
	}
}

EmbreeKernel::EmbreeKernel(const std::vector<Primitive>& primitives)
	: primitives(primitives) {
	build();
}

EmbreeKernel::~EmbreeKernel()
{
	clear();
}

void EmbreeKernel::clear()
{
	primitives.clear();
	rtcDeleteScene(rtcScene);
	rtcDeleteDevice(rtcDevice);
}
void EmbreeKernel::build()
{
	_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
	_MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
	int numTri = (int)primitives.size(), numVer = numTri * 3;
	RTCGeometryFlags geom_flags = RTC_GEOMETRY_STATIC;

	rtcDevice = rtcNewDevice(NULL);

	errorHandler(rtcDeviceGetError(rtcDevice));

	rtcScene = rtcDeviceNewScene(rtcDevice, RTC_SCENE_STATIC, RTC_INTERSECT1);
	geomID = rtcNewTriangleMesh(rtcScene, geom_flags, numTri, numVer, 1);

	rtcScene = rtcDeviceNewScene(rtcDevice, RTC_SCENE_STATIC, RTC_INTERSECT1);
	geomID = rtcNewTriangleMesh(rtcScene, geom_flags, numTri, numVer, 1);

	struct RTCVertex {
		float x, y, z, a;
	};
	struct RTCTriangle {
		int v[3];
	};
	struct Vector4 {
		float x, y, z, a;
		Vector4(const Point3f &p)
			: x(static_cast<float>(p.x)), y(static_cast<float>(p.y)),
			z(static_cast<float>(p.z)), a(0) {}
	};

	RTCVertex *vertices = (RTCVertex *)rtcMapBuffer(rtcScene, geomID, RTC_VERTEX_BUFFER);
	for (int i = 0; i < numTri; i++) {
		const Triangle &tri = *primitives[i].triangle;
		for (int k = 0; k < 3; k++) {
			const Point3f &p = tri.mesh->p[tri.triNumber * 3 + k];
			*(Vector4 *)(&vertices[i * 3 + k]) = Vector4(p);
		}
	}
	rtcUnmapBuffer(rtcScene, geomID, RTC_VERTEX_BUFFER);

	RTCTriangle *triangles = (RTCTriangle *)rtcMapBuffer(rtcScene, geomID, RTC_INDEX_BUFFER);
	for (int i = 0; i < numTri; i++) {
		for (int k = 0; k < 3; k++) {
			triangles[i].v[k] = i * 3 + k;
		}
	}
	rtcUnmapBuffer(rtcScene, geomID, RTC_INDEX_BUFFER);

	rtcCommit(rtcScene);
	errorHandler(rtcDeviceGetError(rtcDevice));
}
bool EmbreeKernel::intersect(const Ray & ray, Intersection * isec) const
{
	RTCRay rtc_ray;
	*(Point3f *)rtc_ray.org = ray.o;
	*(Vector3f *)rtc_ray.dir = ray.d;
	rtc_ray.tnear = ray.t;
	rtc_ray.tfar = ray.tMax;
	rtc_ray.time = 0;
	rtc_ray.mask = -1;
	rtc_ray.geomID = RTC_INVALID_GEOMETRY_ID;
	rtc_ray.primID = RTC_INVALID_GEOMETRY_ID;

	rtcIntersect(rtcScene, rtc_ray);
	if (rtc_ray.primID == -1)
		return false;

	const Primitive &prim = primitives[rtc_ray.primID];
	const Triangle &tri = *prim.triangle;

	Float coordU = rtc_ray.u, coordV = rtc_ray.v;

	// point
	const Point3f &p0 = tri.mesh->p[tri.triNumber * 3];
	const Point3f &p1 = tri.mesh->p[tri.triNumber * 3 + 1];
	const Point3f &p2 = tri.mesh->p[tri.triNumber * 3 + 2];
	Point3f p = (1 - coordU - coordV) * p0 + coordU * p1 + coordV * p2;

	// uv
	const Point2f &uv0 = tri.mesh->uv[tri.triNumber * 3];
	const Point2f &uv1 = tri.mesh->uv[tri.triNumber * 3 + 1];
	const Point2f &uv2 = tri.mesh->uv[tri.triNumber * 3 + 2];
	Point2f uv = (1 - coordU - coordV) * uv0 + coordU * uv1 + coordV * uv2;


	// ns
	const Normal3f &ns0 = tri.mesh->n[tri.triNumber * 3];
	const Normal3f &ns1 = tri.mesh->n[tri.triNumber * 3 + 1];
	const Normal3f &ns2 = tri.mesh->n[tri.triNumber * 3 + 2];
	Normal3f ns = ns0 * (1 - coordU - coordV) + ns1 * coordU + ns2 * coordV;

	// ng
	Normal3f ng = Normal3f(normalize(cross(p1 - p0, p2 - p0)));

	// front
	bool front = dot(ray.o - p0, ns) > 0;
	ns = front ? ns : -ns;
	ng = front ? ng : -ng;

	// t
	Float t = rtc_ray.tfar;

	*isec = Intersection(p, ng, ns, uv, t, front);

	// u, v
	Vector3f u = normalize(p1 - p0);
	Float sign = isec->front ? 1.0f : -1.0f;
	Vector3f v = normalize(cross(isec->ns * sign, isec->u));
	u = normalize(cross(v, isec->ns));
	isec->u = u;
	isec->v = v;

	// primitive
	isec->primitive = &prim;
	return true;
}
ORION_NAMESPACE_END
