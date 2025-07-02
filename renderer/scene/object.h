#pragma once

#include "../../common.h"

namespace lzgl::renderer {

	enum class ObjectType {
		Object,
		Mesh,
		InstancedMesh,
		Scene
	};

	class Object {

	public:

		Object();
		~Object();

		// Position
		void setPosition(glm::vec3 pos);
		glm::vec3 getPosition() const { return mPosition; }

		// Rotation
		void rotateX(float angle);
		void rotateY(float angle);
		void rotateZ(float angle);

		void setAngleX(float angle);
		void setAngleY(float angle);
		void setAngleZ(float angle);

		// Scale
		void setScale(glm::vec3 scale);

		glm::mat4 getModelMatrix() const;
		glm::vec3 getDirection() const;

		// Parent
		void addChild(Object* obj);
		std::vector<Object*> getChildren();
		Object* getParent();

		ObjectType getType() const { return mType; }

	protected:

		glm::vec3 mPosition{ 0.0f };

		float mAngleX{ 0.0f };
		float mAngleY{ 0.0f };
		float mAngleZ{ 0.0f };

		glm::vec3 mScale{ 1.0f };

		// Parent and children
		std::vector<Object*> mChildren{};
		Object* mParent{ nullptr };

		ObjectType mType;
	};
}

