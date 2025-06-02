#include "tools.h"

void Tools::decompose(
	glm::mat4 matrix, 
	glm::vec3& position, 
	glm::vec3& eulerAngle, 
	glm::vec3& scale)
{
	glm::quat quaternion;
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(matrix, scale, quaternion, position, skew, perspective);

	// 1 quaternion to euler
	glm::mat4 rotation = glm::toMat4(quaternion);
	glm::extractEulerAngleXYZ(rotation, eulerAngle.x, eulerAngle.y, eulerAngle.z);

	// 2 euler to degree
	eulerAngle.x = glm::degrees(eulerAngle.x);
	eulerAngle.y = glm::degrees(eulerAngle.y);
	eulerAngle.z = glm::degrees(eulerAngle.z);

}

std::vector<glm::vec4> Tools::getFrustumCornersWorldSpace(const glm::mat4& projView) {

	const auto inv = glm::inverse(projView);

	std::vector<glm::vec4> corners{};

	for (int x = 0; x < 2; x++) {

		for (int y = 0; y < 2; y++) {

			for (int z = 0; z < 2; z++) {

				glm::vec4 ndc = glm::vec4(2.0f * x - 1.0, 2.0f * y - 1.0, 2.0f * z - 1.0, 1.0);
				glm::vec4 p_middle = inv * ndc;
				glm::vec4 p_world = p_middle / p_middle.w;
				corners.push_back(p_world);
			}
		}
	}

	return corners;
}
