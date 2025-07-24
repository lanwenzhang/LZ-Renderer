#pragma once

#include "../common.h"
#include "camera/camera.h"
#include "scene/scene.h"
#include "scene/mesh/mesh.h"
#include "scene/light/directional_light.h"
#include "scene/light/point_light.h"
#include "scene/light/spot_light.h"
#include "scene/light/ambient_light.h"
#include "../wrapper/shader.h"
#include "../wrapper/framebuffer.h"

namespace lzgl::renderer {

	class Renderer {
	public:

		Renderer(); 
		~Renderer();

		void render(Scene* scene, Camera* camera, std::vector<PointLight*> pointLights = {}, unsigned int fbo = 0);
		void renderObject(Object* object, Camera* camera, std::vector<PointLight*> pointLights = {});
		void renderShadowMap(Camera* camera, const std::vector<Mesh*>& meshes, std::vector<PointLight*> pointLights = {});

		void msaaResolve(lzgl::wrapper::Framebuffer* src, lzgl::wrapper::Framebuffer* dst);
		void setClearColor(glm::vec3 color);

		Material* mGlobalMaterial{ nullptr };

		lzgl::wrapper::Texture* generateIrradianceMap(lzgl::wrapper::Texture* equirectTexture, unsigned int resolution);
		lzgl::wrapper::Texture* generatePrefilterMap(lzgl::wrapper::Texture* envCubeMap, unsigned int resolution = 512);
		lzgl::wrapper::Texture* generateBrdfLUT(int resolution = 512);


	private:

		void projectObject(Object* obj);
		Shader* pickShader(MaterialType type);

		void setDepthState(Material* material);
		void setPolygonOffsetState(Material* material);
		void setStencilState(Material* material);
		void setBlendState(Material* material);
		void setFaceCullingState(Material* material);

	private:

		Shader* mScreenShader{ nullptr };
		Shader* mEquirectangularShader{ nullptr };
		Shader* mIrradianceMapShader{ nullptr };
		Shader* mPrefilterShader{ nullptr };
		Shader* mBrdfShader{ nullptr };
		Shader* mPbrShader{ nullptr };

		Shader* mGBufferShader{ nullptr };
		Shader* mLightingShader{ nullptr };

		Shader* mShadowDistanceShader{ nullptr };
		Shader* mPhongPointShadowShader{ nullptr };

		// Objects
		std::vector<Mesh*> mOpacityObjects{};
		std::vector<Mesh*> mTransparentObjects{};
	};
}



