#include <util/parser.h>
#include <util/renderinfo.h>
using namespace orion;
int main() {

	std::string path = "xml/killeroo/killeroo.ori";
	Parser parser(path);
	auto &option = parser.getRenderOption();

	auto scene = option->scene;
	auto integrator = option->integrator;

	std::cout << " [Detected " << numSystemCores() << " cores]" << std::endl;

	integrator->render(*scene);

	auto camera = option->camera;
	auto outputFilm = camera->getFilm()->getAveraged();
	saveAsBmp(outputFilm.get(), "res/killeroo/killeroo.bmp");
}