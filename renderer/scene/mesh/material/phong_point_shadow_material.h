#pragma once

#include "material.h"
#include "../../../../wrapper/texture.h"

namespace lzgl::renderer {

	class PhongPointShadowMaterial :public Material {

	public:
		PhongPointShadowMaterial();
		~PhongPointShadowMaterial();

	public:
		lzgl::wrapper::Texture* mDiffuse{ nullptr };
		lzgl::wrapper::Texture* mSpecularMask{ nullptr };
		float mShiness{ 1.0f };
		float mBias{ 0.0f };
		float mDiskTightness{ 1.0f };
		float mPcfRadius{ 0.0f };
	};
}


