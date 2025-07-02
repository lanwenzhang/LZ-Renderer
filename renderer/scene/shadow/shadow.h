#pragma once
#include "../../common.h"
#include "../../wrapper/framebuffer.h"
#include "../../camera/camera.h"

namespace lzgl::renderer {

	class Shadow {
	public:
		Shadow();
		~Shadow();

		virtual void setRenderTargetSize(int width, int height) = 0;

	public:
		Camera* mCamera{ nullptr };
		lzgl::wrapper::Framebuffer* mRenderTarget{ nullptr };

		float mBias{ 0.0003f };
		float mPcfRadius{ 0.01f };
		float mDiskTightness{ 1.0f };

		float mLightSize{ 0.04f };

	};
}





