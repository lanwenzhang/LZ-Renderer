#pragma once
#include "../core.h"

class Tools {
public:

	// Decompose a matrix to position, rotation, and scale information
	static void decompose(glm::mat4 matrix, glm::vec3& position, glm::vec3& eulerAngle, glm::vec3& scale);

	static std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& projView);
};