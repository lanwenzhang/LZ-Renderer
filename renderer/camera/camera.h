#pragma once

#include "../../common.h"

namespace lzgl::renderer {

	class Camera {

	public:
		Camera();
		~Camera();

		glm::mat4 getViewMatrix();
		virtual glm::mat4 getProjectionMatrix();

		virtual void scale(float deltaScale);
		void setPosition(const glm::vec3& position);
		void updateYaw(float deltaAngle);

	public:
		glm::vec3 mPosition{ 0.0f, 0.0f, 5.0f };
		glm::vec3 mUp{ 0.0f, 1.0f, 0.0f };
		glm::vec3 mRight{ 1.0f, 0.0f, 0.0f };

		float mNear = 0.0f;
		float mFar = 0.0f;

	};
}

