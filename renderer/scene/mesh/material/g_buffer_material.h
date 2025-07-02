#pragma once

#include "material.h"
#include "../../../../wrapper/texture.h"

namespace lzgl::renderer {

	class GBufferMaterial :public Material {
	public:
		GBufferMaterial();
		~GBufferMaterial();

	public:
		lzgl::wrapper::Texture* mDiffuse{ nullptr };
		lzgl::wrapper::Texture* mSpecularMask{ nullptr };
		lzgl::wrapper::Texture* mNormal{ nullptr };
		float mShiness{ 1.0f };
	};
}
