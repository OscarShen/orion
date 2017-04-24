#include <core/geometry.h>
#include <math/linalg.h>
#include <texture/texture.h>
#include <util/texmanager.h>
#include <util/logmanager.h>
#include <texture/checkboard.h>
#include <texture/rendertarget.h>
#include <texture/mixtexture.h>
#include <texture/gridtexture.h>
#include <util/timer.h>
#include <camera/perspective.h>
#include <system/system.h>
#include <shape/triangle.h>
using namespace orion;

System oSystem;

void main()
{
	std::shared_ptr<PerspectiveCamera> camera(new PerspectiveCamera());
	camera->setOrig(Point3f(4.0f, 4.0f, 4.0f));
	camera->setUp(Vector3f(0.0f, 1.0f, 0.0f));
	camera->setLookat(Point3f(0.0f, 0.0f, 0.0f));
	camera->setFov(45.0f);

	std::shared_ptr<RenderTarget> film(new RenderTarget(1960, 1080));
	camera->setRenderTarget(film);

	oSystem.loadScene(""); // hard code _System_
	oSystem.setCamera(camera);
	oSystem.render();
	oSystem.outputFilm("res/aaa.bmp");
}
