#include "renderer.h"
#include "scene/mesh/material/material.h"
#include "scene/mesh/material/screen_material.h"
#include "scene/mesh/material/cube_map_material.h"
#include "scene/mesh/material/pbr_material.h"
#include "scene/mesh/material/g_buffer_material.h"
#include "scene/mesh/material/lighting_material.h"
#include "scene/mesh/instanced_mesh.h"
#include "scene/mesh/material/phong_point_shadow_material.h"
#include "camera/orthographic_camera.h"

#include "scene/shadow/point_light_shadow.h"

namespace lzgl::renderer {

	Renderer::Renderer() {

		mScreenShader = new Shader("shaders/screen/screen.vert", "shaders/screen/screen.frag");
		mEquirectangularShader = new Shader("shaders/skybox/equirectangular.vert", "shaders/skybox/equirectangular.frag");
		mPbrShader = new Shader("shaders/pbr/pbr.vert", "shaders/pbr/pbr.frag");

		mIrradianceMapShader = new Shader("shaders/pbr/ibl/diffuse/irradiance_map.vert", "shaders/pbr/ibl/diffuse/irradiance_map.frag");
		mPrefilterShader = new Shader("shaders/skybox/equirectangular.vert", "shaders/pbr/ibl/specular/prefilter.frag");
		mBrdfShader = new Shader("shaders/screen/screen.vert", "shaders/pbr/ibl/specular/brdf.frag");

		mGBufferShader = new Shader("shaders/deferred/g_buffer.vert", "shaders/deferred/g_buffer.frag");
		mLightingShader = new Shader("shaders/deferred/lighting.vert", "shaders/deferred/lighting.frag");

		mShadowDistanceShader = new Shader("shaders/shadow/shadow_distance.vert", "shaders/shadow/shadow_distance.frag");
		mPhongPointShadowShader = new Shader("shaders/shadow/phong_point_shadow.vert", "shaders/shadow/phong_point_shadow.frag");
	}

	Renderer::~Renderer() {

		delete mScreenShader;
		delete mEquirectangularShader;
		delete mIrradianceMapShader;
		
		delete mPrefilterShader;
		delete mBrdfShader;
		delete mPbrShader;

		delete mGBufferShader;
		delete mLightingShader;

		delete mShadowDistanceShader;
		delete mPhongPointShadowShader;

	}

	void Renderer::msaaResolve(lzgl::wrapper::Framebuffer* src, lzgl::wrapper::Framebuffer* dst) {

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

		// 3.2 Sort objects
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

		// 4 Render shadow
		bool hasPhongPointShadowObject = std::any_of(
			mOpacityObjects.begin(),
			mOpacityObjects.end(),
			[](const auto& object) {
				return object->mMaterial->mType == MaterialType::PhongPointShadowMaterial;
			}
		);


		if (hasPhongPointShadowObject) {

			renderShadowMap(camera, mOpacityObjects, pointLights);
		}


		// 5 Render objects
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
			result = mEquirectangularShader;
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
		case MaterialType::PhongPointShadowMaterial:
			result = mPhongPointShadowShader;
			break;
		default:
			std::cout << "Unkown material type to shader" << std::endl;
			break;
		}

		return result;
	}

	void Renderer::renderObject(Object* object, Camera* camera, std::vector<PointLight*> pointLights) {

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

				shader->setInt("prefilterMap", 5);
				pbrMat->mPrefilterMap->setUnit(5);
				pbrMat->mPrefilterMap->bind();

				shader->setInt("brdfLUT", 6);
				pbrMat->mBrdfLUT->setUnit(6);
				pbrMat->mBrdfLUT->bind();

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
				shader->setInt("sphericalSampler", 0);
				cubeMat->mDiffuse->setUnit(0);
				cubeMat->mDiffuse->bind();

			}
										   break;
			case MaterialType::GBufferMaterial: {

				// pointer type change
				GBufferMaterial* gBufferMat = (GBufferMaterial*)mesh->mMaterial;
				InstancedMesh* im = (InstancedMesh*)mesh;

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
			case MaterialType::PhongPointShadowMaterial: {

				// pointer type change
				PhongPointShadowMaterial* pointShadowMat = (PhongPointShadowMaterial*)material;
				PointLightShadow* pointShadow = (PointLightShadow*)pointLights[0]->mShadow;

				// Texture bind and sampling
				shader->setInt("sampler", 0);
				pointShadowMat->mDiffuse->bind();

				shader->setInt("shadowMapSampler", 1);
				pointShadow->mRenderTarget->mDepthAttachment->setUnit(1);
				pointShadow->mRenderTarget->mDepthAttachment->bind();


				// 3.2.3 MVP matrix
				shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
				shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
				shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

				auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
				shader->setMatrix3x3("normalMatrix", normalMatrix);

				// 3.2.3 Light
				shader->setVector3("pointLight.color", pointLights[0]->mColor);
				shader->setFloat("pointLight.specularIntensity", pointLights[0]->mSpecularIntensity);
				shader->setVector3("pointLight.position", pointLights[0]->getPosition());

				shader->setFloat("shiness", pointShadowMat->mShiness);

				shader->setFloat("pointLight.k2", pointLights[0]->mK2);
				shader->setFloat("pointLight.k1", pointLights[0]->mK1);
				shader->setFloat("pointLight.kc", pointLights[0]->mKc);

				shader->setVector3("ambientColor", glm::vec3(0.1f));

				// 3.2.4 Shadow
				shader->setFloat("bias", pointShadow->mBias);
				shader->setFloat("diskTightness", pointShadow->mDiskTightness);
				shader->setFloat("pcfRadius", pointShadow->mPcfRadius);
				shader->setFloat("far", pointShadow->mCamera->mFar);

				// 3.2.4 Camera
				shader->setVector3("cameraPosition", camera->mPosition);

				// 3.2.5 
				shader->setFloat("opacity", material->mOpacity);


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

	void Renderer::renderShadowMap(Camera* camera, const std::vector<Mesh*>& meshes, std::vector<PointLight*> pointLights) {

		bool isPostProcessPass = true;
		for (int i = 0; i < meshes.size(); i++) {

			auto mesh = meshes[i];
			if (mesh->mMaterial->mType != MaterialType::ScreenMaterial) {

				isPostProcessPass = false;
				break;
			}
		}

		if (isPostProcessPass) {

			return;
		}

		// 2 Save the current state
		GLint preFbo;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &preFbo);

		GLint preViewport[4];
		glGetIntegerv(GL_VIEWPORT, preViewport);

		// 3 Set shadowmap state
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);

		// 4 Render shadow
		PointLightShadow* pointShadow = (PointLightShadow*)pointLights[0]->mShadow;

		auto lightMatrices = pointShadow->getLightMatrices(pointLights[0]->getPosition());

		// 4.3 Bind fbo
		glBindFramebuffer(GL_FRAMEBUFFER, pointShadow->mRenderTarget->mFBO);
		glViewport(0, 0, pointShadow->mRenderTarget->mWidth, pointShadow->mRenderTarget->mHeight);

		// 4.4 Render each cube map shadow
		for (int i = 0; i < 6; i++) {

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, pointShadow->mRenderTarget->mDepthAttachment->getTexture(), 0);

			glClear(GL_DEPTH_BUFFER_BIT);
			mShadowDistanceShader->begin();

			mShadowDistanceShader->setMatrix4x4("lightMatrix", lightMatrices[i]);
			mShadowDistanceShader->setFloat("far", pointShadow->mCamera->mFar);
			mShadowDistanceShader->setVector3("lightPosition", pointLights[0]->getPosition());


			// 4.4.3 Render object
			for (int i = 0; i < meshes.size(); i++) {

				auto mesh = meshes[i];
				auto geometry = mesh->mGeometry;

				glBindVertexArray(geometry->getVao());
				mShadowDistanceShader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());

				// 3.4 Draw
				if (mesh->getType() == ObjectType::InstancedMesh) {

					InstancedMesh* im = (InstancedMesh*)mesh;
					glDrawElementsInstanced(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0, im->mInstanceCount);

				}
				else {

					glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);

				}

			}

			mShadowDistanceShader->end();

		}

		// 5 Recover the state
		glBindFramebuffer(GL_FRAMEBUFFER, preFbo);
		glViewport(preViewport[0], preViewport[1], preViewport[2], preViewport[3]);

	}

	lzgl::wrapper::Texture* Renderer::generateIrradianceMap(lzgl::wrapper::Texture* equirectTexture, unsigned int resolution) {
		
		using namespace lzgl::wrapper;

		// 2. Create cubemap, allocate only level 0
		auto boxGeo = lzgl::renderer::Geometry::createBox(1.0f, true);
		auto cubemap = Texture::createEmptyCubeMapWithMipmap(resolution, GL_RGBA16F, 0, 1);

		auto fbo = Framebuffer::createHDRFbo(resolution, resolution);
		fbo->bind();

		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 captureViews[] = {
			glm::lookAt(glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec3(0,-1,0)),
			glm::lookAt(glm::vec3(0,0,0), glm::vec3(-1,0,0), glm::vec3(0,-1,0)),
			glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,1,0), glm::vec3(0,0,1)),
			glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,-1,0), glm::vec3(0,0,-1)),
			glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,0,1), glm::vec3(0,-1,0)),
			glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,0,-1), glm::vec3(0,-1,0))
		};

	
		mIrradianceMapShader->begin();
		mIrradianceMapShader->setMatrix4x4("projectionMatrix", captureProjection);

		mIrradianceMapShader->setInt("envMap", 0);
		equirectTexture->setUnit(0);
		equirectTexture->bind();

		fbo->setViewport(resolution, resolution);

		glBindVertexArray(boxGeo->getVao());

		for (unsigned int i = 0; i < 6; ++i) {
			mIrradianceMapShader->setMatrix4x4("viewMatrix", captureViews[i]);
			mIrradianceMapShader->setMatrix4x4("modelMatrix", glm::mat4(1.0f));

			fbo->attachCubemapFace(cubemap, i, 0);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glDrawElements(GL_TRIANGLES, boxGeo->getIndicesCount(), GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);

		mIrradianceMapShader->end();
		fbo->unbind();

		// 3. Generate all mipmaps
		cubemap->bind();
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		cubemap->unbind();

		return cubemap;
	}

	lzgl::wrapper::Texture* Renderer::generatePrefilterMap(lzgl::wrapper::Texture* envCubeMap, unsigned int resolution) {
		using namespace lzgl::wrapper;

		unsigned int maxMipLevels = 5;

		auto prefilterMap = Texture::createEmptyCubeMapWithMipmap(
			resolution,
			GL_RGBA16F,
			0,
			maxMipLevels
		);

		auto fbo = Framebuffer::createHDRFbo(resolution, resolution);
		fbo->bind();

		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 captureViews[] = {
			glm::lookAt(glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec3(0,-1,0)),
			glm::lookAt(glm::vec3(0,0,0), glm::vec3(-1,0,0), glm::vec3(0,-1,0)),
			glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,1,0), glm::vec3(0,0,1)),
			glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,-1,0), glm::vec3(0,0,-1)),
			glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,0,1), glm::vec3(0,-1,0)),
			glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,0,-1), glm::vec3(0,-1,0))
		};

		auto boxGeo = lzgl::renderer::Geometry::createBox(1.0f, true);
		mPrefilterShader->begin();
		mPrefilterShader->setMatrix4x4("projectionMatrix", captureProjection);
		mPrefilterShader->setInt("environmentMap", 0);

		envCubeMap->setUnit(0);
		envCubeMap->bind();

		glBindVertexArray(boxGeo->getVao());

		for (unsigned int mip = 0; mip < maxMipLevels; ++mip) {
			unsigned int mipWidth = static_cast<unsigned int>(resolution * std::pow(0.5, mip));
			unsigned int mipHeight = static_cast<unsigned int>(resolution * std::pow(0.5, mip));

			fbo->setViewport(mipWidth, mipHeight);

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			mPrefilterShader->setFloat("roughness", roughness);

			for (unsigned int i = 0; i < 6; ++i) {
				mPrefilterShader->setMatrix4x4("viewMatrix", captureViews[i]);
				mPrefilterShader->setMatrix4x4("modelMatrix", glm::mat4(1.0f));
				fbo->attachCubemapFace(prefilterMap, i, mip);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glDrawElements(GL_TRIANGLES, boxGeo->getIndicesCount(), GL_UNSIGNED_INT, 0);
			}
		}

		glBindVertexArray(0);

		mPrefilterShader->end();
		fbo->unbind();

		// Optional but safe:
		prefilterMap->bind();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, maxMipLevels - 1);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		prefilterMap->unbind();

		return prefilterMap;
	}

	lzgl::wrapper::Texture* Renderer::generateBrdfLUT(int resolution) {
		
		using namespace lzgl::wrapper;

		auto fbo = Framebuffer::createHDRFbo(resolution, resolution);
		fbo->bind();

		unsigned int brdfLUTTex;
		glGenTextures(1, &brdfLUTTex);
		glBindTexture(GL_TEXTURE_2D, brdfLUTTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, resolution, resolution, 0, GL_RG, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTex, 0);

		auto quadGeo = lzgl::renderer::Geometry::createScreenPlane();

		fbo->setViewport(resolution, resolution);

		mBrdfShader->begin();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(quadGeo->getVao());
		glDrawElements(GL_TRIANGLES, quadGeo->getIndicesCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		mBrdfShader->end();
		fbo->unbind();

		return Texture::createFromID(brdfLUTTex, resolution, resolution, GL_TEXTURE_2D, 0);
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

}

