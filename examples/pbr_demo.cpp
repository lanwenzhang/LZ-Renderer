#include "../common.h"
#include "../core/application.h"
#include "../wrapper/check_error.h"
#include "../wrapper/shader.h"
#include "../wrapper/framebuffer.h"
#include "../wrapper/texture.h"

#include "../loader/assimp_loader.h"
#include "../renderer/camera/perspective_camera.h"
#include "../renderer/camera/game_camera_control.h"
#include "../renderer/scene/scene.h"
#include "../renderer/scene/mesh/mesh.h"
#include "../renderer/scene/mesh/instanced_mesh.h"
#include "../renderer/scene/mesh/geometry/geometry.h"
#include "../renderer/scene/mesh/material/pbr_material.h"
#include "../renderer/scene/mesh/material/screen_material.h"
#include "../renderer/scene/mesh/material/cube_map_material.h"
#include "../renderer/scene/light/point_light.h"
#include "../renderer/renderer.h"
#include "../renderer/post_processing/bloom.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"


// 1 Global variables
// 1.1 Window
int WIDTH = 2560;
int HEIGHT = 1080;
glm::vec3 clearColor{};

// 1.2 Render
lzgl::renderer::Scene* sceneOff = nullptr;
std::vector<lzgl::renderer::PointLight*> pointLights{};
lzgl::renderer::PbrMaterial* material = nullptr;
lzgl::renderer::Camera* camera = nullptr;
lzgl::renderer::GameCameraControl* cameraControl = nullptr;
lzgl::renderer::Renderer* renderer = nullptr;

lzgl::renderer::Bloom* bloom = nullptr;
lzgl::wrapper::Framebuffer* fboMulti = nullptr;
lzgl::wrapper::Framebuffer* fboResolve = nullptr;
lzgl::renderer::Scene* scene = nullptr;
lzgl::renderer::ScreenMaterial* screenMat = nullptr;

// --------------------------------------------

// 2 Mouse and keyboard event
void OnResize(int width, int height) {

    GL_CALL(glViewport(0, 0, width, height));
}

void OnKey(int key, int action, int mods) {

    cameraControl->onKey(key, action, mods);
}

void OnMouse(int button, int action, int mods) {

    double x, y;
    glApp->getCursorPosition(&x, &y);
    cameraControl->onMouse(button, action, x, y);

}

void OnCursor(double xpos, double ypos) {

    cameraControl->onCursor(xpos, ypos);
}

void OnScroll(double offset) {

    cameraControl->onScroll(offset);
}


// --------------------------------------------

// 3 Prepare render objects and camera

void prepare() {

    fboMulti = lzgl::wrapper::Framebuffer::createMultiSampleHDRFbo(WIDTH, HEIGHT);
    fboResolve = lzgl::wrapper::Framebuffer::createHDRFbo(WIDTH, HEIGHT);

    renderer = new lzgl::renderer::Renderer();
    bloom = new lzgl::renderer::Bloom(WIDTH, HEIGHT);
    sceneOff = new lzgl::renderer::Scene();
    scene = new lzgl::renderer::Scene();

    // Pass 01
    // Gun model
    auto gun = lzgl::loader::AssimpLoader::load("assets/textures/pbr/gun.fbx");
    gun->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
    gun->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    gun->rotateZ(180.0f);
    sceneOff->addChild(gun);

    // Cube map
    auto boxGeo = lzgl::renderer::Geometry::createBox(1.0f);
    auto boxMat = new lzgl::renderer::CubeMaterial();
    boxMat->mDiffuse = lzgl::wrapper::Texture::createExrTexture("assets/textures/pbr/qwantani_dusk_2_4k.exr");
    auto boxMesh = new lzgl::renderer::Mesh(boxGeo, boxMat);
    sceneOff->addChild(boxMesh);

    // Pass 02
    auto sgeo = lzgl::renderer::Geometry::createScreenPlane();
    screenMat = new lzgl::renderer::ScreenMaterial();
    screenMat->mScreenTexture = fboResolve->mColorAttachment;
    auto smesh = new lzgl::renderer::Mesh(sgeo, screenMat);
    scene->addChild(smesh);

    // 4 Create light
    glm::vec3 lightPositions[] = {
            glm::vec3(-3.0f,  3.0f, 10.0f),
            glm::vec3(3.0f,  3.0f, 10.0f),
            glm::vec3(-3.0f, -3.0f, 10.0f),
            glm::vec3(3.0f, -3.0f, 10.0f),
    };
    glm::vec3 lightColors[] = {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f)
    };
    for (int i = 0; i < 4; i++) {
        auto pointLight = new lzgl::renderer::PointLight();
        pointLight->setPosition(lightPositions[i]);
        pointLight->mColor = lightColors[i];
        pointLights.push_back(pointLight);
    }
}


void prepareCamera() {

    camera = new lzgl::renderer::PerspectiveCamera(60.0f, (float)glApp->getWidth() / glApp->getHeight(), 0.1f, 1000.0f);
    camera->setPosition(glm::vec3(0.0f, 0.0f, 20.0f));
    cameraControl = new lzgl::renderer::GameCameraControl();
    cameraControl->setCamera(camera);
    cameraControl->setSensitivity(0.4f);
    cameraControl->setSpeed(0.1f);
}

// --------------------------------------------

// 4 Prepare UI

void initIMGUI() {

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(glApp->getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void renderIMGUI() {

    // 1 Initial
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 2 GUI widget
    ImGui::Begin("MaterialEditor");
    ImGui::SliderFloat("exposure:", &screenMat->mExposure, 0.0f, 10.0f);
    ImGui::SliderFloat("Threshold:", &bloom->mThreshold, 0.0f, 100.0f);
    ImGui::SliderFloat("BloomAttenuation:", &bloom->mBloomAttenuation, 0.0f, 1.0f);
    ImGui::SliderFloat("BloomIntensity:", &bloom->mBloomIntensity, 0.0f, 1.0f);
    ImGui::SliderFloat("BloomRadius:", &bloom->mBloomRadius, 0.0f, 1.0f);

    ImGui::End();

    // 3 Render
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(glApp->getWindow(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


// 5 Render
int main() {

    // 5.1 Initial the window
    if (!glApp->init(WIDTH, HEIGHT)) {
        return -1;
    }

    // 5.2 Size and keyboard callback
    glApp->setResizeCallback(OnResize);
    glApp->setKeyBoardCallback(OnKey);
    glApp->setMouseCallback(OnMouse);
    glApp->setCursorCallback(OnCursor);
    glApp->setScrollCallback(OnScroll);

    // 5.3 Set openGl rendering viewport and clear canvas color
    GL_CALL(glViewport(0, 0, WIDTH, HEIGHT));
    GL_CALL(glClearColor(0.0f, 0.18f, 0.65f, 1.0f));

    // 4 Set up camera, objects, UI
    prepareCamera();
    prepare();
    initIMGUI();

    // 4 Set window loop
    while (glApp->update()) {

        cameraControl->autoYaw(0.0015f);
        cameraControl->update();

        renderer->setClearColor(clearColor);
        renderer->render(sceneOff, camera, pointLights, fboMulti->mFBO);
        renderer->msaaResolve(fboMulti, fboResolve);
        bloom->doBloom(fboResolve);
        renderer->render(scene, camera, pointLights);

        renderIMGUI();
    }

    glApp->destroy();

    return 0;
}