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
using namespace orion;

System oSystem;

void main()
{
	std::shared_ptr<PerspectiveCamera> camera(new PerspectiveCamera());
	camera->setOrig(Point3f(1.0f, 0.0f, 0.0f));
	camera->setUp(Vector3f(0.0f, 1.0f, 0.0f));
	camera->setLookat(Point3f(0.0f, 0.0f, 0.0f));
	camera->setFov(45.0f);

	std::shared_ptr<RenderTarget> film(new RenderTarget(800, 600));
	camera->setRenderTarget(film);

	oSystem.setCamera(camera);
	oSystem.render();
	oSystem.outputFilm("aaa.bmp");
}
