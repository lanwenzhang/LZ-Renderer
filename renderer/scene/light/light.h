#pragma once

#include "../../../common.h"
#include "../object.h"
#include "../shadow/shadow.h"

namespace lzgl::renderer {
	
	class Light :public Object {
	public:
		Light();
		~Light();

	public:
		glm::vec3 mColor{ 1.0f };
		float mSpecularIntensity{ 1.0f };
		float mIntensity{ 1.0f };
		Shadow* mShadow{ nullptr };
	};
}

