#pragma once

#include "material.h"
#include "../../../../wrapper/texture.h"

namespace lzgl::renderer {

	class ScreenMaterial :public Material {
	public:
		ScreenMaterial();
		~ScreenMaterial();

	public:
		lzgl::wrapper::Texture* mScreenTexture{ nullptr };
		float mExposure{ 1.0f };
	};
}



