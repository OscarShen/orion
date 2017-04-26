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
#include <util/envvariable.h>
using namespace orion;

System oSystem;

void main()
{
	oSystem.loadScene(""); // hard code _System_

	Timer::inst()->reset();
	oSystem.render();
	std::cout << "elapsed time : " << Timer::inst()->getElaspedTime() << std::endl;

	oSystem.outputFilm("res/test.bmp");
}
