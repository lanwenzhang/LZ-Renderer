#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../application/stb_image.h"

#define TINYEXR_IMPLEMENTATION
#include "../application/tinyexr.h"

std::map<std::string, Texture*> Texture::mTextureCache{};

Texture* Texture::createTexture(const std::string& path, unsigned int unit) {
    
    // 1 Check if generate before
    auto iter = mTextureCache.find(path);
    if (iter != mTextureCache.end()) {

        return iter->second;
    }


    auto texture = new Texture(path, unit, GL_SRGB_ALPHA);
    mTextureCache[path] = texture;

    return texture;
}

Texture* Texture::createTextureFromMemory(const std::string& path, unsigned int unit,unsigned char* dataIn, uint32_t widthIn,uint32_t heightIn) {

    auto iter = mTextureCache.find(path);
    if (iter != mTextureCache.end()) {

        return iter->second;
    }

    auto texture = new Texture(unit, dataIn, widthIn, heightIn, GL_SRGB_ALPHA);

    mTextureCache[path] = texture;

    return texture;
}

Texture* Texture::createColorAttachment(unsigned int width, unsigned int height,unsigned int unit) {

    return new Texture(width, height, unit);
}

Texture* Texture::createDepthStencilAttachment(unsigned int width, unsigned int height, unsigned int unit) {

    Texture* dsTex = new Texture();

    unsigned int depthStencil;
    glGenTextures(1, &depthStencil);
    glBindTexture(GL_TEXTURE_2D, depthStencil);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencil, 0);

    dsTex->mTexture = depthStencil;
    dsTex->mWidth = width;
    dsTex->mHeight = height;
    dsTex->mUnit = unit;

    return dsTex;
}

Texture* Texture::createDepthAttachment(unsigned int width, unsigned int height, unsigned int unit) {

    Texture* depthTex = new Texture();

    unsigned int depth;
    glGenTextures(1, &depth);
    glBindTexture(GL_TEXTURE_2D, depth);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glBindTexture(GL_TEXTURE_2D, 0);

    depthTex->mTexture = depth;
    depthTex->mWidth = width;
    depthTex->mHeight = height;
    depthTex->mUnit = unit;

    return depthTex;

}

Texture* Texture::createGBufferAttachment(unsigned int width, unsigned int height, unsigned int unit, unsigned int internalFormat, unsigned int dataType) {

    Texture* tex = new Texture();

    GLuint glTex;
    glGenTextures(1, &glTex);
    glBindTexture(GL_TEXTURE_2D + unit, glTex);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, dataType, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    tex->mTexture = glTex;
    tex->mWidth = width;
    tex->mHeight = height;
    tex->mUnit = unit;

    return tex;

}

Texture* Texture::createMultiSampleTexture(unsigned int width, unsigned int height, unsigned int samples, unsigned int format, unsigned int unit) {

    Texture* tex = new Texture();

    GLuint glTex;
    glGenTextures(1, &glTex);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, glTex);

    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_TRUE);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    tex->mTexture = glTex;
    tex->mWidth = width;
    tex->mHeight = height;
    tex->mUnit = unit;
    tex->mTextureTarget = GL_TEXTURE_2D_MULTISAMPLE;

    return tex;
}

Texture* Texture::createHDRTexture(unsigned int width, unsigned int height,unsigned int unit) {

    Texture* tex = new Texture();

    GLuint glTex;
    glGenTextures(1, &glTex);
    glBindTexture(GL_TEXTURE_2D, glTex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glBindTexture(GL_TEXTURE_2D, 0);

    tex->mTexture = glTex;
    tex->mWidth = width;
    tex->mHeight = height;
    tex->mUnit = unit;
    tex->mTextureTarget = GL_TEXTURE_2D;

    return tex;

}

Texture* Texture::createNearestTexture(std::string path) {

    auto texture = new Texture(path, 0);

    glBindTexture(GL_TEXTURE_2D, texture->mTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

Texture* Texture::createExrTexture(std::string path) {

    Texture* tex = new Texture();

    float* data = nullptr;
    int width, height;
    const char* err = nullptr;
    int ret;

    ret = LoadEXR(&data, &width, &height, path.c_str(), &err);
    if (ret != TINYEXR_SUCCESS) {

        if (err) {

            std::cerr << "Error Loading Exr:" << err << std::endl;
            FreeEXRErrorMessage(err);
        }

        return nullptr;
    }

    int channels = 4;
    for (int y = 0; y < height / 2; ++y) {

        int opposite_y = height - 1 - y;
        for (int x = 0; x < width * channels; x++) {

            std::swap(data[width * channels * y + x], data[width * channels * opposite_y + x]);
        }
    }


    GLuint glTex;
    glGenTextures(1, &glTex);
    glBindTexture(GL_TEXTURE_2D, glTex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGBA, GL_FLOAT, data);

    // 5 Set filter 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // 6 Set wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    tex->mTexture = glTex;
    tex->mWidth = width;
    tex->mHeight = height;
    tex->mUnit = 0;

    return tex;

}

Texture* Texture::createExrCubeMap(std::vector<std::string> paths) {

    Texture* tex = new Texture();

    GLuint glTex;
    glGenTextures(1, &glTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, glTex);

    int width = 0, height = 0;
    float* data = nullptr;
    const char* err = nullptr;
    int ret = 0;
    float maxNumber = 0.0;

    for (int i = 0; i < paths.size(); i++) {
     
        ret = LoadEXR(&data, &width, &height, paths[i].c_str(), &err);
        for (int k = 0; k < width * height * 4; k++) {
            maxNumber = data[k] > maxNumber ? data[k] : maxNumber;
        }
        if (data != nullptr) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, width, height, 0, GL_RGBA, GL_FLOAT, data);
        }
        else {
            std::cout << "Error: CubeMap Texture failed to load at path¡¡£­" << paths[i] << std::endl;
        }
    }

    std::cout << maxNumber << std::endl;
  
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    tex->mTexture = glTex;
    tex->mWidth = width;
    tex->mHeight = height;
    tex->mUnit = 0;
    tex->mTextureTarget = GL_TEXTURE_CUBE_MAP;

    return tex;
}

Texture::Texture() {}

Texture::Texture(const std::string &path, unsigned int unit, unsigned int internalFormat){
    
    mUnit = unit;
    // 1 stb read the texture
    // 1,1 Define variable
    int channels;

    // 1.2 Flip y-axis
    stbi_set_flip_vertically_on_load(true);

    // 1.3 Read texture
    unsigned char* data = stbi_load(path.c_str(), &mWidth, &mHeight, &channels, STBI_rgb_alpha);


    // 2 Generate
    glGenTextures(1, &mTexture);
    glActiveTexture(GL_TEXTURE0 + mUnit);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    // 3 Send data
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // 4 Release data
    stbi_image_free(data);

    // 5 Set filter 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // 6 Set wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::Texture(unsigned int unit, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn, unsigned int internalFormat) 
{
    mUnit = unit;
    // 1 stb read the texture
    // 1,1 Define variable
    int channels;

    // 1.2 Flip y-axis
    stbi_set_flip_vertically_on_load(true);

    // 1.3 Read texture
    // Assimp rules: png or jpg file, height = 0, width is the size of image
    uint32_t dataInSize = 0;
    if (!heightIn) {

        dataInSize = widthIn;
    }
    else {

        dataInSize = widthIn * heightIn * 4;
    }

    unsigned char* data = stbi_load_from_memory(dataIn, dataInSize, &mWidth, &mHeight, &channels, STBI_rgb_alpha);


    // 2 Generate
    glGenTextures(1, &mTexture);
    glActiveTexture(GL_TEXTURE0 + mUnit);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    // 3 Send data
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // 4 Release data
    stbi_image_free(data);

    // 5 Set filter 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // 6 Set wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


}

Texture::Texture(unsigned int width, unsigned int height, unsigned int unit, unsigned int internalFormat){

    mWidth = width;
    mHeight = height;
    mUnit = unit;

    glGenTextures(1, &mTexture);
    glActiveTexture(GL_TEXTURE0 + mUnit);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::Texture(const std::vector<std::string>& paths, unsigned int unit, unsigned int internalFormat) {

    mUnit = unit;
    mTextureTarget = GL_TEXTURE_CUBE_MAP;

    stbi_set_flip_vertically_on_load(false);

    // 1 Create cubemap texture object
    glGenTextures(1, &mTexture);
    glActiveTexture(GL_TEXTURE0 + mUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);

    // 2 stb read the texture
    // 2,1 Define variable
    int channels;
    int width = 0, height = 0;
    unsigned char* data = nullptr;
    for (int i = 0; i < paths.size(); i++) {

        data = stbi_load(paths[i].c_str(), &width, &height, &channels, STBI_rgb_alpha);

        if (data != nullptr) {

            // 2.2 Send data
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {

            std::cout << "Error: CubeMap Texture failed to load at path - " << paths[i] << std::endl;
            stbi_image_free(data);
        }
    }
    
    // 6 Set filter 
    glTexParameteri(mTextureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(mTextureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // 7 Set wrapping
    glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

Texture::~Texture(){
    if (mTexture != 0) {
        glDeleteTextures(1, &mTexture);
    }
}

void Texture::bind() {

    glActiveTexture(GL_TEXTURE0 + mUnit);
    glBindTexture(mTextureTarget, mTexture);
}