#include "texmanager.h"

namespace orion {

	template <>
	std::shared_ptr<TexManager> Singleton<TexManager>::_instance = nullptr;

}