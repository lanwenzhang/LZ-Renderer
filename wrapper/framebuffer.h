#pragma once

#include "../common.h"
#include "texture.h"

namespace lzgl::wrapper {
	
	class Framebuffer {
	public:

		Framebuffer();
		Framebuffer(unsigned int width, unsigned int height);
		~Framebuffer();

		static Framebuffer* createMultiSampleFbo(unsigned int width, unsigned int height, unsigned int samples);
		static Framebuffer* createHDRFbo(unsigned int width, unsigned int height);
		static Framebuffer* createHDRBloomFbo(unsigned int width, unsigned int height);
		static Framebuffer* createMultiSampleHDRFbo(unsigned int width, unsigned int height, unsigned int samples = 4);
		static Framebuffer* createGBufferFbo(unsigned int width, unsigned int height);
		static Framebuffer* createPointShadowFbo(unsigned int width, unsigned int height);

		void bind();
		void unbind();
		void setViewport(int width, int height);
		void attachCubemapFace(lzgl::wrapper::Texture* cubemap, unsigned int face, unsigned int mip);

	public:

		unsigned int mWidth{ 0 };
		unsigned int mHeight{ 0 };

		unsigned int mFBO{ 0 };
		Texture* mColorAttachment{ nullptr };
		Texture* mDepthStencilAttachment{ nullptr };
		Texture* mDepthAttachment{ nullptr };

		std::vector<Texture*> mGBufferAttachment{};
	};
}

