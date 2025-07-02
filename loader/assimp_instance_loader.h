#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../common.h"
#include "../renderer/scene/object.h"
#include "../renderer/scene/mesh/instanced_mesh.h"
#include "../wrapper/texture.h"


namespace lzgl::loader {

	class AssimpInstanceLoader {
	public:

		static  lzgl::renderer::Object* load(const std::string& path, int instanceCount);

	private:

		static void processNode(
			aiNode* ainode,
			lzgl::renderer::Object* parent,
			const aiScene* scene,
			const std::string& rootPath,
			int instanceCount
		);

		static lzgl::renderer::InstancedMesh* processMesh(
			aiMesh* aimesh,
			const aiScene* scene,
			const std::string& rootPath,
			int instanceCount
		);

		static lzgl::wrapper::Texture* processTexture(
			const aiMaterial* aimat,
			const aiTextureType& type,
			const aiScene* scene,
			const std::string& rootPath
		);


		static glm::mat4 getMat4f(aiMatrix4x4 value);

	};

}
