#pragma once

#include "material.h"
#include "../texture.h"


class LightingMaterial :public Material {
public:
	LightingMaterial();
	~LightingMaterial();

public:

	Texture* mPosition{ nullptr };
	Texture* mNormal{ nullptr };
	Texture* mAlbedoSpec{ nullptr };
	int mDisplayMode{ 0 };
};