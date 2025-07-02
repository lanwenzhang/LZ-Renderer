#include "instanced_mesh.h"
#include <algorithm>

namespace lzgl::renderer {

	InstancedMesh::InstancedMesh(
		Geometry* geometry,
		Material* material,
		unsigned int instanceCount) :Mesh(geometry, material) {

		mType = ObjectType::InstancedMesh;
		mInstanceCount = instanceCount;
		mInstanceMatrices.resize(instanceCount);

		glGenBuffers(1, &mMatrixVbo);
		glBindBuffer(GL_ARRAY_BUFFER, mMatrixVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * mInstanceCount, mInstanceMatrices.data(), GL_DYNAMIC_DRAW);

		glBindVertexArray(mGeometry->getVao());
		glBindBuffer(GL_ARRAY_BUFFER, mMatrixVbo);

		for (int i = 0; i < 4; i++) {

			glEnableVertexAttribArray(4 + i);
			glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * i * 4));
			glVertexAttribDivisor(4 + i, 1);
		}

		glBindVertexArray(0);
	}


	InstancedMesh::~InstancedMesh() {}

	void InstancedMesh::updateMatrices() {

		glBindBuffer(GL_ARRAY_BUFFER, mMatrixVbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * mInstanceCount, mInstanceMatrices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


}

