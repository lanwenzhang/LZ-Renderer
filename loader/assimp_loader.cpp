#include "assimp_loader.h"
#include "../tools/tools.h"
#include "../renderer/scene/mesh/material/pbr_material.h"

namespace lzgl::loader {

	lzgl::renderer::Object* AssimpLoader::load(
		const std::string& path,
		lzgl::wrapper::Texture* envCubeMap,
		lzgl::wrapper::Texture* prefilterMap,
		lzgl::wrapper::Texture* brdfLUT
	) {

		std::size_t lastIndex = path.find_last_of("//");
		auto rootPath = path.substr(0, lastIndex + 1);

		lzgl::renderer::Object* rootNode = new lzgl::renderer::Object();

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

		// Check whether readfile succeed
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {

			std::cout << "Error: Model Read Failed" << std::endl;
			return nullptr;
		}

		processNode(scene->mRootNode, rootNode, scene, rootPath, envCubeMap, prefilterMap, brdfLUT);

		return rootNode;
	}

	void AssimpLoader::processNode(
		aiNode* ainode, 
		lzgl::renderer::Object* parent, 
		const aiScene* scene, 
		const std::string& rootPath,
		lzgl::wrapper::Texture* envCubeMap,
		lzgl::wrapper::Texture* prefilterMap,
		lzgl::wrapper::Texture* brdfLUT
	) {

		// 1 Generate node and link parent
		lzgl::renderer::Object* node = new lzgl::renderer::Object();
		parent->addChild(node);

		// 2 Get node local transformation
		glm::mat4 localMatrix = getMat4f(ainode->mTransformation);
		glm::vec3 position, eulerAngle, scale;
		lzgl::tools::Tools::decompose(localMatrix, position, eulerAngle, scale);
		node->setPosition(position);
		node->setAngleX(eulerAngle.x);
		node->setAngleY(eulerAngle.y);
		node->setAngleZ(eulerAngle.z);
		node->setScale(scale);

		// 3 Check mesh
		for (int i = 0; i < ainode->mNumMeshes; i++) {

			int meshID = ainode->mMeshes[i];
			aiMesh* aimesh = scene->mMeshes[meshID];

			auto mesh = processMesh(aimesh, scene, rootPath, envCubeMap, prefilterMap, brdfLUT);

			node->addChild(mesh);
		}

		// 4 Check child
		for (int i = 0; i < ainode->mNumChildren; i++) {

			processNode(ainode->mChildren[i], node, scene, rootPath, envCubeMap, prefilterMap, brdfLUT);
		}
	}

	lzgl::renderer::Mesh* AssimpLoader::processMesh(
		aiMesh* aimesh, 
		const aiScene* scene, 
		const std::string& rootPath,
		lzgl::wrapper::Texture* envCubeMap,
		lzgl::wrapper::Texture* prefilterMap,
		lzgl::wrapper::Texture* brdfLUT
	) {

		std::vector<float> positions;
		std::vector<float> normals;
		std::vector<float> uvs;
		std::vector<unsigned int> indices;
		std::vector<float> tangents;

		// 1 Positions information
		for (int i = 0; i < aimesh->mNumVertices; i++) {

			positions.push_back(aimesh->mVertices[i].x);
			positions.push_back(aimesh->mVertices[i].y);
			positions.push_back(aimesh->mVertices[i].z);

			normals.push_back(aimesh->mNormals[i].x);
			normals.push_back(aimesh->mNormals[i].y);
			normals.push_back(aimesh->mNormals[i].z);

			// number 0 uvs are texture uv
			if (aimesh->mTextureCoords[0]) {

				uvs.push_back(aimesh->mTextureCoords[0][i].x);
				uvs.push_back(aimesh->mTextureCoords[0][i].y);
			}
			else {
				uvs.push_back(0.0f);
				uvs.push_back(0.0f);
			}

			if (aimesh->HasTangentsAndBitangents()) {

				tangents.push_back(aimesh->mTangents[i].x);
				tangents.push_back(aimesh->mTangents[i].y);
				tangents.push_back(aimesh->mTangents[i].z);
			}
		}


		// 2 Indices information
		for (int i = 0; i < aimesh->mNumFaces; i++) {

			aiFace face = aimesh->mFaces[i];

			for (int j = 0; j < face.mNumIndices; j++) {

				indices.push_back(face.mIndices[j]);

			}
		}

		// 3 Create geometry
		auto geometry = new lzgl::renderer::Geometry(positions, normals, uvs, indices, tangents);
		auto material = new lzgl::renderer::PbrMaterial();

		// 4 Create texture
		if (aimesh->mMaterialIndex >= 0) {

			aiMaterial* aiMat = scene->mMaterials[aimesh->mMaterialIndex];

			material->mAlbedo = processTexture(aiMat, aiTextureType_BASE_COLOR, scene, rootPath);
			if (!material->mAlbedo) {

				material->mAlbedo = new lzgl::wrapper::Texture("assets/textures/pbr/Cerberus_A.jpg", 0, GL_SRGB_ALPHA);
			}

			material->mNormal = processTexture(aiMat, aiTextureType_NORMALS, scene, rootPath);
			if (!material->mNormal) {

				material->mNormal = lzgl::wrapper::Texture::createNearestTexture("assets/textures/pbr/Cerberus_N.jpg");
			}

			material->mRoughness = processTexture(aiMat, aiTextureType_DIFFUSE_ROUGHNESS, scene, rootPath);
			if (!material->mRoughness) {

				material->mRoughness = lzgl::wrapper::Texture::createNearestTexture("assets/textures/pbr/Cerberus_R.jpg");
			}

			material->mMetallic = processTexture(aiMat, aiTextureType_METALNESS, scene, rootPath);
			if (!material->mMetallic) {

				material->mMetallic = lzgl::wrapper::Texture::createNearestTexture("assets/textures/pbr/Cerberus_M.jpg");
			}

			material->mIrradianceIndirect = envCubeMap;
			material->mPrefilterMap = prefilterMap;
			material->mBrdfLUT = brdfLUT;
		}

		return new lzgl::renderer::Mesh(geometry, material);

	}

	lzgl::wrapper::Texture* AssimpLoader::processTexture(const aiMaterial* aimat, const aiTextureType& type, const aiScene* scene, const std::string& rootPath) {

		lzgl::wrapper::Texture* texture = nullptr;

		aiString aipath;
		aimat->Get(AI_MATKEY_TEXTURE(type, 0), aipath);

		if (!aipath.length) {
			return nullptr;
		}

		// Check if fbx has texture
		const aiTexture* aitexture = scene->GetEmbeddedTexture(aipath.C_Str());
		if (aitexture) {

			unsigned char* dataIn = reinterpret_cast<unsigned char*>(aitexture->pcData);
			uint32_t widthIn = aitexture->mWidth;
			uint32_t heightIn = aitexture->mHeight;
			texture = lzgl::wrapper::Texture::createTextureFromMemory(aipath.C_Str(), 0, dataIn, widthIn, heightIn);
		}
		else {

			std::string fullPath = rootPath + aipath.C_Str();
			texture = lzgl::wrapper::Texture::createTexture(fullPath, 0);
		}

		return texture;
	}

	glm::mat4 AssimpLoader::getMat4f(aiMatrix4x4 value) {

		glm::mat4 to(
			value.a1, value.a2, value.a3, value.a4,
			value.b1, value.b2, value.b3, value.b4,
			value.c1, value.c2, value.c3, value.c4,
			value.d1, value.d2, value.d3, value.d4
		);

		return to;
	}



}

