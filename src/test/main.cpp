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
#include <system/system.h>
#include <shape/triangle.h>
#include <util/envvariable.h>
using namespace orion;

System oSystem;

void main()
{
	Timer::inst()->reset();
	oSystem.setUp();
	std::cout << "setup time : " << Timer::inst()->getElaspedTime() << "ms" << std::endl;

	Timer::inst()->reset();
	oSystem.render();

	oSystem.outputFilm("res/bunny.bmp");
}
