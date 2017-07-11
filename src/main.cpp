#include <util/parser.h>
#include <util/renderinfo.h>
using namespace orion;
int main() {

	std::string path = "xml/dragon/dragon_metal.ori";
	Parser parser(path);
	auto &option = parser.getRenderOption();

	auto scene = option->scene;
	auto integrator = option->integrator;

	std::cout << " [Detected " << numSystemCores() << " cores]" << std::endl;

	integrator->render(*scene);

	auto camera = option->camera;
	auto outputFilm = camera->getFilm()->getAveraged();
	saveAsBmp(outputFilm.get(), "res/dragon/dragon_metal.bmp");
}