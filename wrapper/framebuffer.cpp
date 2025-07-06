#include "framebuffer.h"

namespace lzgl::wrapper {

    Framebuffer::Framebuffer() {}

    Framebuffer::Framebuffer(unsigned int width, unsigned int height) {

        mWidth = width;
        mHeight = height;

        // 1 Create FBO
        glGenFramebuffers(1, &mFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

        // 2 Create color attachment
        mColorAttachment = Texture::createColorAttachment(mWidth, mHeight, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment->getTexture(), 0);

        // 3 Create depth and stencil attachment 
        mDepthStencilAttachment = Texture::createDepthStencilAttachment(mWidth, mHeight, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthStencilAttachment->getTexture(), 0);

        // 4 Check error
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Error:FrameBuffer is not complete!" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);


    }

    Framebuffer* Framebuffer::createMultiSampleFbo(unsigned int width, unsigned int height, unsigned int samples) {

        Framebuffer* fb = new Framebuffer();

        unsigned int fbo;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        auto colorAttachment = Texture::createMultiSampleTexture(width, height, samples, GL_RGBA, 0);
        auto dsAttachment = Texture::createMultiSampleTexture(width, height, samples, GL_DEPTH24_STENCIL8, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, colorAttachment->getTexture(), 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, dsAttachment->getTexture(), 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        fb->mFBO = fbo;
        fb->mDepthAttachment = dsAttachment;
        fb->mColorAttachment = colorAttachment;
        fb->mWidth = width;
        fb->mHeight = height;

        return fb;

    }

    Framebuffer* Framebuffer::createHDRFbo(unsigned int width, unsigned int height) {

        Framebuffer* fb = new Framebuffer();

        unsigned int fbo;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        auto colorAttachment = Texture::createHDRTexture(width, height, 0);
        auto dsAttachment = Texture::createDepthStencilAttachment(width, height, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment->getTexture(), 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, dsAttachment->getTexture(), 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        fb->mFBO = fbo;
        fb->mDepthStencilAttachment = dsAttachment;
        fb->mColorAttachment = colorAttachment;
        fb->mWidth = width;
        fb->mHeight = height;

        return fb;
    }

    Framebuffer* Framebuffer::createHDRBloomFbo(unsigned int width, unsigned int height) {

        Framebuffer* fb = new Framebuffer();

        unsigned int fbo;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        auto colorAttachment = Texture::createHDRTexture(width, height, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment->getTexture(), 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        fb->mFBO = fbo;
        fb->mColorAttachment = colorAttachment;
        fb->mWidth = width;
        fb->mHeight = height;

        return fb;

    }

    Framebuffer* Framebuffer::createMultiSampleHDRFbo(unsigned int width, unsigned int height, unsigned int samples) {
        Framebuffer* fb = new Framebuffer();

        unsigned int fbo;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        auto colorAttachment = Texture::createMultiSampleTexture(width, height, samples, GL_RGB16F, 0);
        auto dsAttachment = Texture::createMultiSampleTexture(width, height, samples, GL_DEPTH24_STENCIL8, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, colorAttachment->getTexture(), 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, dsAttachment->getTexture(), 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        fb->mFBO = fbo;
        fb->mDepthStencilAttachment = dsAttachment;
        fb->mColorAttachment = colorAttachment;
        fb->mWidth = width;
        fb->mHeight = height;

        return fb;
    }

    Framebuffer* Framebuffer::createGBufferFbo(unsigned int width, unsigned int height) {

        Framebuffer* fb = new Framebuffer();

        unsigned int fbo;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        auto positionTex = Texture::createGBufferAttachment(width, height, 0, GL_RGBA16F, GL_FLOAT);
        auto normalTex = Texture::createGBufferAttachment(width, height, 0, GL_RGBA16F, GL_FLOAT);
        auto albedoSpecTex = Texture::createGBufferAttachment(width, height, 0, GL_SRGB8_ALPHA8, GL_UNSIGNED_BYTE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, positionTex->getTexture(), 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalTex->getTexture(), 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, albedoSpecTex->getTexture(), 0);

        unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        glDrawBuffers(3, attachments);

        // 2 Depth attachment
        auto depthTex = Texture::createDepthAttachment(width, height, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex->getTexture(), 0);


        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {

            std::cout << "Framebuffer not complete!" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        fb->mFBO = fbo;
        fb->mGBufferAttachment.push_back(positionTex);
        fb->mGBufferAttachment.push_back(normalTex);
        fb->mGBufferAttachment.push_back(albedoSpecTex);
        fb->mDepthAttachment = depthTex;
        fb->mWidth = width;
        fb->mHeight = height;

        return fb;
    }

    Framebuffer* Framebuffer::createPointShadowFbo(unsigned int width, unsigned int height) {
        Framebuffer* fb = new Framebuffer();

        unsigned int fbo;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        Texture* depthAttachment = Texture::createDepthAttachmentCubeMap(width, height, 0);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            depthAttachment->getTexture(),
            0
        );
        glDrawBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        fb->mFBO = fbo;
        fb->mDepthAttachment = depthAttachment;
        fb->mWidth = width;
        fb->mHeight = height;

        return fb;
    }

    void Framebuffer::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    }

    void Framebuffer::unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::setViewport(int width, int height)
    {
        glViewport(0, 0, width, height);
        mWidth = width;
        mHeight = height;
    }

    void Framebuffer::attachCubemapFace(lzgl::wrapper::Texture* cubemap, unsigned int face, unsigned int mip)
    {
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
            cubemap->getTexture(),
            mip
        );
    }

    Framebuffer::~Framebuffer() {

        if (mFBO) {

            glDeleteFramebuffers(1, &mFBO);

        }
        if (mColorAttachment != nullptr) {

            delete mColorAttachment;
        }
        if (mDepthStencilAttachment != nullptr) {

            delete mDepthStencilAttachment;
        }
    }
}

