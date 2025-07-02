#pragma once

#include "mesh.h"

namespace lzgl::renderer {
	class InstancedMesh :public Mesh {
	public:
		InstancedMesh(Geometry* geometry, Material* material, unsigned int instanceCount);
		~InstancedMesh();

		void InstancedMesh::updateMatrices();
	public:
		unsigned int	mInstanceCount{ 0 };
		std::vector<glm::mat4>	mInstanceMatrices{};
		unsigned int	mMatrixVbo{ 0 };
	};
}

