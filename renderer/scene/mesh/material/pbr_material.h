#pragma once

#include "material.h"
#include "../../../../wrapper/texture.h"

namespace lzgl::renderer {

	class PbrMaterial :public Material {
	public:
		PbrMaterial();
		~PbrMaterial();

	public:

		lzgl::wrapper::Texture* mAlbedo{ nullptr };
		lzgl::wrapper::Texture* mRoughness{ nullptr };
		lzgl::wrapper::Texture* mNormal{ nullptr };
		lzgl::wrapper::Texture* mMetallic{ nullptr };

		lzgl::wrapper::Texture* mIrradianceIndirect{ nullptr };
		lzgl::wrapper::Texture* mPrefilterMap{ nullptr };
		lzgl::wrapper::Texture* mBrdfLUT{ nullptr };
	};
}



