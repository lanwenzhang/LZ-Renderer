#pragma once

#include "../material.h"
#include "../../texture.h"


class PbrMaterial :public Material {
public:
	PbrMaterial();
	~PbrMaterial();

public:

	Texture* mAlbedo{ nullptr };
	Texture* mRoughness{ nullptr };
	Texture* mNormal{ nullptr };
	Texture* mMetallic{ nullptr };
	Texture* mIrradianceIndirect{ nullptr };

};