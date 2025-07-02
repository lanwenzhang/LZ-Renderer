#pragma once

#include "material.h"
#include "../../../../wrapper/texture.h"

namespace lzgl::renderer {

	class LightingMaterial :public Material {
	public:
		LightingMaterial();
		~LightingMaterial();

	public:

		lzgl::wrapper::Texture* mPosition{ nullptr };
		lzgl::wrapper::Texture* mNormal{ nullptr };
		lzgl::wrapper::Texture* mAlbedoSpec{ nullptr };
		int mDisplayMode{ 0 };
	};
}
