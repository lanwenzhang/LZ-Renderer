#include "renderer.h"
#include "../material/screenMaterial.h"
#include "../material/cubeMaterial.h"
#include "../material/advanced/pbrMaterial.h"
#include "../material/gBufferMaterial.h"
#include "../material/lightingMaterial.h"
#include "../mesh/instancedMesh.h"
#include "../../application/camera/orthographicCamera.h"

Renderer::Renderer() {

	mScreenShader = new Shader("assets/shaders/screen.vert", "assets/shaders/screen.frag");
	mCubeShader = new Shader("assets/shaders/cube.vert", "assets/shaders/cube.frag");
	mPbrShader = new Shader("assets/shaders/advanced/pbr/pbr.vert", "assets/shaders/advanced/pbr/pbr.frag");

	mGBufferShader = new Shader("assets/shaders/gBuffer.vert", "assets/shaders/gBuffer.frag");
	mLightingShader = new Shader("assets/shaders/lighting.vert", "assets/shaders/lighting.frag");
}

Renderer::~Renderer() {

}

void Renderer::setClearColor(glm::vec3 color) {

	glClearColor(color.r, color.g, color.b, 1.0);
}

void Renderer::setDepthState(Material* material) {

	if (material->mDepthTest) {

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(material->mDepthFunc);
	}
	else {

		glDisable(GL_DEPTH_TEST);
	}

	// 2.2 Depth write
	if (material->mDepthWrite) {

		glDepthMask(GL_TRUE);
	}
	else {

		glDepthMask(GL_FALSE);
	}

}

void Renderer::setPolygonOffsetState(Material* material) {

	if (material->mPolygonOffset) {

		glEnable(material->mPolygonOffsetType);
		glPolygonOffset(material->mFactor, material->mUnit);

	}
	else {
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_POLYGON_OFFSET_LINE);

	}

}

void Renderer::setStencilState(Material* material) {

	if (material->mStencilTest) {

		glEnable(GL_STENCIL_TEST);

		glStencilOp(material->mSFail, material->mZFail, material->mZPass);
		glStencilMask(material->mStencilMask);
		glStencilFunc(material->mStencilFunc, material->mStencilRef, material->mStencilFuncMask);

	}
	else {

		glDisable(GL_STENCIL_TEST);
	}
}

void Renderer::setBlendState(Material* material) {

	if (material->mBlend) {

		glEnable(GL_BLEND);
		glBlendFunc(material->mSFactor, material->mDFactor);
	}
	else {

		glDisable(GL_BLEND);
	}
}

void Renderer::setFaceCullingState(Material* material) {

	if (material->mFaceCulling) {

		glEnable(GL_CULL_FACE);
		glFrontFace(material->mFrontFace);
		glCullFace(material->mCullFace);

	}
	else {

		glDisable(GL_CULL_FACE);
	}
}

void Renderer::msaaResolve(Framebuffer* src, Framebuffer* dst) {

	glBindFramebuffer(GL_READ_FRAMEBUFFER, src->mFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst->mFBO);
	glBlitFramebuffer(0, 0, src->mWidth, src->mHeight, 0, 0, dst->mWidth, dst->mHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void Renderer::render(Scene* scene, Camera* camera, std::vector<PointLight*> pointLights, unsigned int fbo) {

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// 1 Depth and stencil test
	// 1.1 Depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	// 1.2 Polygon offset
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_POLYGON_OFFSET_LINE);

	// 1.3 Stencil test
	glEnable(GL_STENCIL_TEST);

	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glStencilMask(0xFF);

	// 1.4 Blend
	glDisable(GL_BLEND);


	// 2 Clear canvas
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// 3 Category and sort objects
	// 3.1 Clear two containers
	mOpacityObjects.clear();
	mTransparentObjects.clear();

	projectObject(scene);

	// 3.3 Sort objects
	std::sort(
		mTransparentObjects.begin(),
		mTransparentObjects.end(),
		[camera](const Mesh* a, const Mesh* b) {


			auto viewMatrix = camera->getViewMatrix();

			auto modelMatrixA = a->getModelMatrix();
			auto worldPositionA = modelMatrixA * glm::vec4(0.0, 0.0, 0.0, 1.0);
			auto cameraPositionA = viewMatrix * worldPositionA;

			auto modelMatrixB = b->getModelMatrix();
			auto worldPositionB = modelMatrixB * glm::vec4(0.0, 0.0, 0.0, 1.0);
			auto cameraPositionB = viewMatrix * worldPositionB;

			return cameraPositionA.z < cameraPositionB.z;
		}
	);

	// 4 Render objects
	for (int i = 0; i < mOpacityObjects.size(); i++) {

		renderObject(mOpacityObjects[i], camera, pointLights);

	}

	for (int i = 0; i < mTransparentObjects.size(); i++) {

		renderObject(mTransparentObjects[i], camera, pointLights);

	}
}

void Renderer::projectObject(Object* obj) {

	if (obj->getType() == ObjectType::Mesh || obj->getType() == ObjectType::InstancedMesh) {

		Mesh* mesh = (Mesh*)obj;
		auto material = mesh->mMaterial;
		if (material->mBlend) {

			mTransparentObjects.push_back(mesh);
		}
		else {

			mOpacityObjects.push_back(mesh);
		}
	}

	auto children = obj->getChildren();
	for (int i = 0; i < children.size(); i++) {

		projectObject(children[i]);
	}
}

Shader* Renderer::pickShader(MaterialType type) {

	Shader* result = nullptr;

	switch (type) {

	case MaterialType::ScreenMaterial:
		result = mScreenShader;
		break;
	case MaterialType::CubeMaterial:
		result = mCubeShader;
		break;
	case MaterialType::PbrMaterial:
		result = mPbrShader;
		break;
	case MaterialType::GBufferMaterial:
		result = mGBufferShader;
		break;
	case MaterialType::LightingMaterial:
		result = mLightingShader;
		break;
	default:
		std::cout << "Unkown material type to shader" << std::endl;
		break;
	}

	return result;
}

void Renderer::renderObject(Object* object, Camera* camera,std::vector<PointLight*> pointLights) {

	// 1 Render only mesh
	if (object->getType() == ObjectType::Mesh || object->getType() == ObjectType::InstancedMesh) {

		// 1.1 It is a mesh
		auto mesh = (Mesh*)object;
		auto geometry = mesh->mGeometry;

		Material* material = nullptr;
		if (mGlobalMaterial != nullptr) {

			material = mGlobalMaterial;
		}
		else {

			material = mesh->mMaterial;
		}


		// 2 Set rendering status
		setDepthState(material);
		setPolygonOffsetState(material);
		setStencilState(material);
		setBlendState(material);
		setFaceCullingState(material);

		// 3.1 Choose shader
		Shader* shader = pickShader(material->mType);

		// 3.2 Update uniform
		// 3.2.1 Create program
		shader->begin();

		switch (material->mType) {
		case MaterialType::ScreenMaterial: {

			ScreenMaterial* screenMat = (ScreenMaterial*)material;

			// Texture bind and sampling
			shader->setInt("screemTexSampler", 0);
			screenMat->mScreenTexture->bind();

			shader->setFloat("texWidth", 2560);
			shader->setFloat("texHeight", 1080);

			shader->setFloat("exposure", screenMat->mExposure);
		}
			break;
		case MaterialType::PbrMaterial: {

			// pointer type change
			PbrMaterial* pbrMat = (PbrMaterial*)material;

			//// Texture bind and sampling
			//shader->setInt("sampler", 0);

			// 3.2.3 MVP matrix
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			shader->setMatrix3x3("normalMatrix", normalMatrix);

			// 3.2.4 Camera
			shader->setVector3("cameraPosition", camera->mPosition);
			
			// 3.2.5 Pbr
			shader->setInt("albedoTex", 0);
			pbrMat->mAlbedo->setUnit(0);
			pbrMat->mAlbedo->bind();

			shader->setInt("normalTex", 1);
			pbrMat->mNormal->setUnit(1);
			pbrMat->mNormal->bind();

			shader->setInt("roughnessTex", 2);
			pbrMat->mRoughness->setUnit(2);
			pbrMat->mRoughness->bind();

			shader->setInt("metallicTex", 3);
			pbrMat->mMetallic->setUnit(3);
			pbrMat->mMetallic->bind();

			shader->setInt("irradianceMap", 4);
			pbrMat->mIrradianceIndirect->setUnit(4);
			pbrMat->mIrradianceIndirect->bind();


			for (int i = 0; i < pointLights.size(); i++) {

				shader->setVector3("pointLights[" + std::to_string(i) + "].color", pointLights[i]->mColor);
				shader->setVector3("pointLights[" + std::to_string(i) + "].position", pointLights[i]->getPosition());
			}
		}
			break;
		case MaterialType::CubeMaterial: {

			CubeMaterial* cubeMat = (CubeMaterial*)material;
			mesh->setPosition(camera->mPosition);
			// MVP matrix
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			// Texture bind and sampling
			shader->setInt("cubeSampler", 0);
			cubeMat->mDiffuse->bind();
		}
		    break;
		case MaterialType::GBufferMaterial: {

			// pointer type change
			GBufferMaterial* gBufferMat = (GBufferMaterial*)mesh->mMaterial;
			InstancedMesh* im = (InstancedMesh*)mesh;

			im->sortMatrices(camera->getViewMatrix());
			im->updateMatrices();

			// Texture bind and sampling
			shader->setInt("diffuse", 0);
			gBufferMat->mDiffuse->setUnit(0);
			gBufferMat->mDiffuse->bind();

			shader->setInt("specular", 1);
			gBufferMat->mSpecularMask->setUnit(1);
			gBufferMat->mSpecularMask->bind();

			shader->setInt("normalTex", 2);
			gBufferMat->mNormal->setUnit(2);
			gBufferMat->mNormal->bind();


			// 3.2.3 MVP matrix
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			//auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			//shader->setMatrix3x3("normalMatrix", normalMatrix);


			// 3.2.4 Camera
			shader->setVector3("cameraPosition", camera->mPosition);

		}
			break;
		case MaterialType::LightingMaterial: {

			// pointer type change
			LightingMaterial* lightingMat = (LightingMaterial*)mesh->mMaterial;


			// Texture bind and sampling
			shader->setInt("gPosition", 0);
			lightingMat->mPosition->setUnit(0);
			lightingMat->mPosition->bind();

			shader->setInt("gNormal", 1);
			lightingMat->mNormal->setUnit(1);
			lightingMat->mNormal->bind();

			shader->setInt("gAlbedoSpec", 2);
			lightingMat->mAlbedoSpec->setUnit(2);
			lightingMat->mAlbedoSpec->bind();

			shader->setInt("displayMode", lightingMat->mDisplayMode);

			// 3.2.4 Camera
			shader->setVector3("cameraPosition", camera->mPosition);

			for (int i = 0; i < pointLights.size(); i++) {

				shader->setVector3("pointLights[" + std::to_string(i) + "].color", pointLights[i]->mColor);
				shader->setVector3("pointLights[" + std::to_string(i) + "].position", pointLights[i]->getPosition());
			}

		}
			break;
		default:
			break;
		}

		// 3.3 VAO
		glBindVertexArray(geometry->getVao());

		// 3.4 Draw
		if (object->getType() == ObjectType::InstancedMesh) {

			InstancedMesh* im = (InstancedMesh*)mesh;
			glDrawElementsInstanced(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0, im->mInstanceCount);

		}
		else {

			glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);

		}
	}
}


