#pragma once

#include "shadow.h"

namespace lzgl::renderer {

	class PointLightShadow : public Shadow {

	public:
		PointLightShadow();
		~PointLightShadow();

		void setRenderTargetSize(int width, int height)override;

		std::vector<glm::mat4> getLightMatrices(glm::vec3 lightPos);
	};

}

