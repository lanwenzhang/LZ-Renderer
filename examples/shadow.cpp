#include "../glframework/core.h"
#include "../glframework/shader.h"
#include "../wrapper/checkError.h"
#include "../application/Application.h"
#include "../glframework/texture.h"

#include "../application/camera/perspectiveCamera.h"
#include "../application/camera/trackBallCameraControl.h"
#include "../application/camera/gameCameraControl.h"

#include "../glframework/geometry.h"
#include "../glframework//material/screenMaterial.h"
#include "../glframework/material/phongPointShadowMaterial.h"

#include "../glframework/mesh/mesh.h"
#include "../glframework/renderer/renderer.h"
#include "../glframework/light/directionalLight.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "../glframework/scene.h"
#include "../application/assimpLoader.h"

#include "../glframework/framebuffer/framebuffer.h"


// 1 Variables
// 1.1 Window
int WIDTH = 2560;
int HEIGHT = 1080;

glm::vec3 clearColor{};

// 1.2 Objects
std::vector<PointLight*> pointLights{};
Camera* camera = nullptr;
GameCameraControl* cameraControl = nullptr;


// 1.3 Render
Renderer* renderer = nullptr;
Scene* scene = nullptr;
Scene* sceneOff = nullptr;

Framebuffer* fbo = nullptr;

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

    fbo = new Framebuffer(WIDTH, HEIGHT);

    renderer = new Renderer();
    scene = new Scene();
    sceneOff = new Scene();

    // Render pass
    auto roomGeo = Geometry::createBox(20, true);
    auto roomMat = new PhongPointShadowMaterial();
    roomMat->mDiffuse = new Texture("assets/textures/wall.png", 0, GL_SRGB_ALPHA);;
    roomMat->mShiness = 32;

    auto roomMesh = new Mesh(roomGeo, roomMat);
    sceneOff->addChild(roomMesh);

    auto boxGeo = Geometry::createBox(2, false);
    auto boxMat = new PhongPointShadowMaterial();
    boxMat->mDiffuse = new Texture("assets/textures/box.png", 0, GL_SRGB_ALPHA);;
    boxMat->mShiness = 32;

    std::vector<glm::vec3> positions = {
        {-3.5683, -2.7041, 7.0375},
        {-3.0233, 5.2973, 2.0394},
        {3.3926, 6.9407, 3.3232},
        {9.8015, -9.8498, 7.3877},
        {0.8651, -7.4239, 1.6375},
        {2.8383, 6.4861, 8.7925},
        {3.0698, -8.5348, 4.8735},
        {6.1476, 2.2643, 5.0017},
        {6.7825, -4.7561, 4.6169},
        {-8.0400, -1.3347, 7.2126},
    };

    for (int i = 0; i < positions.size(); i++) {
        auto boxMesh = new Mesh(boxGeo, boxMat);
        boxMesh->setPosition(positions[i]);
        sceneOff->addChild(boxMesh);
    }


    // Post-process pass
    auto sgeo = Geometry::createScreenPlane();
    auto smat = new ScreenMaterial();
    smat->mScreenTexture = fbo->mColorAttachment;
    auto smesh = new Mesh(sgeo, smat);
    scene->addChild(smesh);

    // 4 Create light
    auto pointLight = new PointLight();
    pointLight->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    pointLight->mSpecularIntensity = 1.0f;
    pointLight->mK2 = 0.0017f;
    pointLight->mK1 = 0.07f;
    pointLight->mKc = 1.0f;
    pointLights.push_back(pointLight);
  
}

void prepareCamera() {

    camera = new PerspectiveCamera(
        60.0f,
        (float)glApp->getWidth() / glApp->getHeight(),
        0.1f,
        1000.0f);

    cameraControl = new GameCameraControl();
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
    ImGui::Begin("Shadow");
    ImGui::SliderFloat("bias:", &pointLights[0]->mShadow->mBias, 0.0f, 0.01f, "%.4f");
    ImGui::SliderFloat("tightness:", &pointLights[0]->mShadow->mDiskTightness, 0.0f, 5.0f, "%.3f");
    ImGui::SliderFloat("pcfRadius:", &pointLights[0]->mShadow->mPcfRadius, 0.0f, 1.0f, "%.4f");


    //ImGui::SliderFloat("Light size", &dirLight->mShadow->mLightSize, 0.0, 10.0);
    ImGui::End();

    // 3 Render
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(glApp->getWindow(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main() {

    // 1 Initial the window
    if (!glApp->init(WIDTH, HEIGHT)) {
        return -1;
    }

    // 2 Size and keyboard callback
    glApp->setResizeCallback(OnResize);
    glApp->setKeyBoardCallback(OnKey);
    glApp->setMouseCallback(OnMouse);
    glApp->setCursorCallback(OnCursor);
    glApp->setScrollCallback(OnScroll);

    // 3 Set openGl rendering viewport and clear canvas color
    GL_CALL(glViewport(0, 0, WIDTH, HEIGHT));
    GL_CALL(glClearColor(0.0f, 0.18f, 0.65f, 1.0f));

    // 4 Set up camera, objects, UI
    prepareCamera();
    prepare();
    initIMGUI();

    // 4 Set window loop
    while (glApp->update()) {

        cameraControl->update();

        renderer->setClearColor(clearColor);

        renderer->render(sceneOff, camera, pointLights, fbo->mFBO);
        renderer->render(scene, camera, pointLights);

        renderIMGUI();
    }

    glApp->destroy();

    return 0;
}