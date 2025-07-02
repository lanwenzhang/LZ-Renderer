#pragma once

#include "light.h"

namespace lzgl::renderer {

	class DirectionalLight :public Light {
	public:
		DirectionalLight();
		~DirectionalLight();
	};
}

