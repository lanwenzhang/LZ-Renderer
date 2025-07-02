#include "../common.h"
#include "../core/application.h"
#include "../wrapper/check_error.h"
#include "../wrapper/shader.h"
#include "../wrapper/framebuffer.h"
#include "../wrapper/texture.h"

#include "../loader/assimp_instance_loader.h"
#include "../renderer/camera/perspective_camera.h"
#include "../renderer/camera/game_camera_control.h"
#include "../renderer/scene/scene.h"
#include "../renderer/scene/mesh/mesh.h"
#include "../renderer/scene/mesh/instanced_mesh.h"
#include "../renderer/scene/mesh/geometry/geometry.h"
#include "../renderer/scene/mesh/material/g_buffer_material.h"
#include "../renderer/scene/mesh/material/lighting_material.h"
#include "../renderer/scene/mesh/material/screen_material.h"
#include "../renderer/scene/light/point_light.h"
#include "../renderer/renderer.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

// 1 Global variables
// 1.1 Window
int WIDTH = 2560;
int HEIGHT = 1080;

// 1.2 Objects
lzgl::renderer::GBufferMaterial* gMat = nullptr;
lzgl::renderer::LightingMaterial* lMat = nullptr;
lzgl::renderer::Geometry* lGeo = nullptr;
std::vector<lzgl::renderer::PointLight*> pointLights{};
lzgl::renderer::Camera* camera = nullptr;
lzgl::renderer::GameCameraControl* cameraControl = nullptr;

// 1.3 Render
glm::vec3 clearColor{};

// 1.3.1 Geometry pass
lzgl::renderer::Renderer* renderer = nullptr;
lzgl::renderer::Scene* sceneOff = nullptr;

// 1.3.2 Lighting pass
lzgl::wrapper::Framebuffer* gBuffer = nullptr;
lzgl::renderer::Scene* scene = nullptr;
int displayMode = 0;


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


void setInstanceMatrix(lzgl::renderer::Object* obj, int index, glm::mat4 matrix) {

    if (obj->getType() == lzgl::renderer::ObjectType::InstancedMesh) {

        lzgl::renderer::InstancedMesh* im = (lzgl::renderer::InstancedMesh*)obj;
        im->mInstanceMatrices[index] = matrix;

    }

    auto children = obj->getChildren();
    for (int i = 0; i < children.size(); i++) {

        setInstanceMatrix(children[i], index, matrix);
    }
}

void updateInstanceMatrix(lzgl::renderer::Object* obj) {

    if (obj->getType() == lzgl::renderer::ObjectType::InstancedMesh) {

        lzgl::renderer::InstancedMesh* im = (lzgl::renderer::InstancedMesh*)obj;
        im->updateMatrices();

    }

    auto children = obj->getChildren();
    for (int i = 0; i < children.size(); i++) {

        updateInstanceMatrix(children[i]);
    }
}

void setInstanceMaterial(lzgl::renderer::Object* obj, lzgl::renderer::Material* material) {

    if (obj->getType() == lzgl::renderer::ObjectType::InstancedMesh) {

        lzgl::renderer::InstancedMesh* im = (lzgl::renderer::InstancedMesh*)obj;
        im->mMaterial = material;

    }

    auto children = obj->getChildren();
    for (int i = 0; i < children.size(); i++) {

        setInstanceMaterial(children[i], material);
    }
}

void prepare() {

    gBuffer = lzgl::wrapper::Framebuffer::createGBufferFbo(WIDTH, HEIGHT);
    renderer = new lzgl::renderer::Renderer();
    sceneOff = new lzgl::renderer::Scene();
    scene = new lzgl::renderer::Scene();

    // Geometry pass
    int rNum = 3;
    int cNum = 3;

    auto cyborgModel = lzgl::loader::AssimpInstanceLoader::load("assets/cyborg/cyborg.obj", rNum * cNum);


    glm::mat4 translate;
    glm::mat4 scale;
    glm::mat4 transform;

    for (int r = 0; r < rNum; r++) {

        for (int c = 0; c < cNum; c++) {

            // 1 translate
            translate = glm::translate(glm::mat4(1.0f), glm::vec3(c * 3.0f - 3.0f, -0.5f, r * 3.0f - 3.0f));

            // 2 rotate
            scale = glm::scale(glm::vec3(0.5f, 0.5f, 0.5f));

            transform = translate * scale;

            setInstanceMatrix(cyborgModel, r * cNum + c, transform);

        }
    }

    updateInstanceMatrix(cyborgModel);

    gMat = new lzgl::renderer::GBufferMaterial();
    gMat->mDiffuse = new lzgl::wrapper::Texture("assets/cyborg/cyborg_diffuse.png", 0);
    gMat->mSpecularMask = lzgl::wrapper::Texture::createNearestTexture("assets/cyborg/cyborg_specular.png");
    gMat->mNormal = lzgl::wrapper::Texture::createNearestTexture("assets/cyborg/cyborg_normal.png");
    setInstanceMaterial(cyborgModel, gMat);
    sceneOff->addChild(cyborgModel);


    // Lighting pass
    lGeo = lzgl::renderer::Geometry::createScreenPlane();
    lMat = new lzgl::renderer::LightingMaterial();
    lMat->mPosition = gBuffer->mGBufferAttachment[0];
    lMat->mNormal = gBuffer->mGBufferAttachment[1];
    lMat->mAlbedoSpec = gBuffer->mGBufferAttachment[2];
    lMat->mDisplayMode = displayMode;
    lMat->mDepthTest = false;
    auto lightingMesh = new lzgl::renderer::Mesh(lGeo, lMat);
    scene->addChild(lightingMesh);


    for (int i = 0; i < 32; i++) {
        auto pointLight = new lzgl::renderer::PointLight();
        pointLight->setPosition(glm::vec3(static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0),
            static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0),
            static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0)));
        pointLight->mColor = (glm::vec3(1.0f, 1.0f, 1.0f));
        pointLights.push_back(pointLight);
    }
}

void prepareCamera() {

    camera = new lzgl::renderer::PerspectiveCamera(80.0f, (float)glApp->getWidth() / glApp->getHeight(), 0.1f, 1000.0f);

    cameraControl = new lzgl::renderer::GameCameraControl();
    cameraControl->setCamera(camera);
    cameraControl->setSensitivity(0.4f);
    cameraControl->setSpeed(0.01f);
}

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
    ImGui::Begin("Buffer Attachment Editor");
    const char* modes[] = { "Final Lighting", "Position", "Normal", "Albedo", "Specular" };
    ImGui::Combo("Display Mode", &displayMode, modes, IM_ARRAYSIZE(modes));
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

    prepareCamera();
    prepare();
    initIMGUI();

    // 4 Set window loop
    while (glApp->update()) {

        cameraControl->update();
        renderer->setClearColor(clearColor);
        lMat->mDisplayMode = displayMode;

        // 4.1 Geometry pass
        renderer->render(sceneOff, camera, {}, gBuffer->mFBO);

        // 4.2 Lighting pass
        renderer->render(scene, camera, pointLights, 0);

        renderIMGUI();
    }

    glApp->destroy();

    return 0;
}