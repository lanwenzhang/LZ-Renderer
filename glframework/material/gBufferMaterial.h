#pragma once

#include "material.h"
#include "../texture.h"


class GBufferMaterial :public Material {
public:
	GBufferMaterial();
	~GBufferMaterial();

public:
	Texture* mDiffuse{ nullptr };
	Texture* mSpecularMask{ nullptr };
	Texture* mNormal{ nullptr };
	float mShiness{ 1.0f };
};