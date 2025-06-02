#pragma once

#include "../core.h"
#include "../mesh/mesh.h"
#include "../../application/camera/camera.h"
#include "../light/directionalLight.h"
#include "../light/pointLight.h"
#include "../light/spotLight.h"
#include "../light/ambientLight.h"
#include "../shader.h"
#include "../scene.h"
#include "../framebuffer/framebuffer.h"

class Renderer {

public:

	Renderer();

	~Renderer();

	void render(Scene* scene,Camera* camera, std::vector<PointLight*> pointLights = {}, unsigned int fbo = 0);

	void renderObject(Object* object, Camera* camera, std::vector<PointLight*> pointLights = {});

	void msaaResolve(Framebuffer* src, Framebuffer* dst);

	void setClearColor(glm::vec3 color);

	Material* mGlobalMaterial{ nullptr };

private:

	void projectObject(Object* obj);
	Shader* pickShader(MaterialType type);

	void setDepthState(Material* material);
	void setPolygonOffsetState(Material* material);
	void setStencilState(Material* material);
	void setBlendState(Material* material);
	void setFaceCullingState(Material* material);

private:

	// Shaders
	Shader* mScreenShader{ nullptr };
	Shader* mCubeShader{ nullptr };
	Shader* mPbrShader{ nullptr };
	Shader* mGBufferShader{ nullptr };
	Shader* mLightingShader{ nullptr };

	// Objects
	std::vector<Mesh*> mOpacityObjects{};
	std::vector<Mesh*> mTransparentObjects{};

};