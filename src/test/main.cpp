#include <core/geometry.h>
#include <math/linalg.h>
using namespace ort;

void main() {
	Normal3f a;
	Normal3f b(1, 2, 3);
	Normal3f c(b);
	a = c * 4.0f + b * 0;

	std::cout << "a:" << a << std::endl;
	std::cout << "b:" << b << std::endl;
	std::cout << "a+b:" << a + b << std::endl;
	std::cout << "length square b:" << lengthSquare(b) << std::endl;
	std::cout << "length b:" << length(b) << std::endl;
	std::cout << "unit a:" << normalize(a) << std::endl;
}