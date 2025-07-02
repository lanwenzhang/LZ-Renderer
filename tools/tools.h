#pragma once
#include "../common.h"

namespace lzgl::tools {
	
	class Tools {
	public:
		static void decompose(glm::mat4 matrix, glm::vec3& position, glm::vec3& eulerAngle, glm::vec3& scale);
		static std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& projView);
	};
}
