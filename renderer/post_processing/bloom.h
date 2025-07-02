#pragma once

#include "../../common.h"
#include "../scene/mesh/geometry/geometry.h"
#include "../../wrapper/framebuffer.h"
#include "../../wrapper/shader.h"

namespace lzgl::renderer {

	class Bloom {

	public:
		Bloom(int width, int height, int minResolution = 32);
		~Bloom();

		void doBloom(lzgl::wrapper::Framebuffer* srcFbo);

	private:
		void extractBright(lzgl::wrapper::Framebuffer* src, lzgl::wrapper::Framebuffer* dst);
		void downSample(lzgl::wrapper::Framebuffer* src, lzgl::wrapper::Framebuffer* dst);
		void upSample(lzgl::wrapper::Framebuffer* target, lzgl::wrapper::Framebuffer* lowerResFbo, lzgl::wrapper::Framebuffer* higherResFbo);
		void merge(lzgl::wrapper::Framebuffer* target, lzgl::wrapper::Framebuffer* origin, lzgl::wrapper::Framebuffer* bloom);

	public:

		std::vector<lzgl::wrapper::Framebuffer*> mDownSamples{};
		std::vector<lzgl::wrapper::Framebuffer*> mUpSamples{};

		int mWidth{ 0 };
		int mHeight{ 0 };
		int mMipLevels{ 0 };

		Geometry* mQuad{ nullptr };
		lzgl::wrapper::Framebuffer* mOriginFbo{ nullptr };

		Shader* mExtractBrightShader{ nullptr };
		Shader* mUpSampleShader{ nullptr };
		Shader* mMergeShader{ nullptr };

		float mThreshold{ 1.0f };
		float mBloomRadius{ 0.1f };
		float mBloomAttenuation{ 1.0f };
		float mBloomIntensity{ 1.0f };
	};
}

