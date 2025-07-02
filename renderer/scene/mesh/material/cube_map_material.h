#pragma once

#include "material.h"
#include "../../../../wrapper/texture.h"

namespace lzgl::renderer {

	class CubeMaterial :public Material {
	public:
		CubeMaterial();
		~CubeMaterial();

	public:
		lzgl::wrapper::Texture* mDiffuse{ nullptr };
	};
}
