#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glengine/square.hpp>
#include "glengine/disc.hpp"
#include <glengine/cube.hpp>
#include <glengine/texture.hpp>
#include "glengine/scene.hpp"
#include "glengine/renderer.hpp"
#include "glengine/window.hpp"
#include "glengine/shaderManager.hpp"
#include "tp01/customScene.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <imgui/imfilebrowser.h>

using namespace GLEngine;

extern const char* _resources_directory;

int main() {

    Window window(1280, 720);
    CustomScene scene;
    Renderer renderer;
    ShaderManager shaderManager;

    Shader* nprShader = new Shader(
            std::string(_resources_directory).append("shaders/default.vert").c_str(),
            std::string(_resources_directory).append("shaders/nrp.frag").c_str()
    );
    shaderManager.addShader("NPR", nprShader);
    Shader* textureShader = new Shader(
            std::string(_resources_directory).append("shaders/default.vert").c_str(),
            std::string(_resources_directory).append("shaders/texture.frag").c_str()
    );
    shaderManager.addShader("Texture", textureShader);

    Mesh * cube = new Cube();
    scene.addMesh(cube);
    cube->setPosition(glm::vec3(0.f, 0.f, 0.f));
    //cube->setScale(glm::vec3(2.f, 2.f, 2.f));

    Texture* box2 = Texture::load(std::string(_resources_directory).append("textures/box2/box2.jpg").c_str());
    Texture* box2_diffus = Texture::load(std::string(_resources_directory).append("textures/box2/box2-diffus.png").c_str());
    Texture* box2_diffus_mask = Texture::load(std::string(_resources_directory).append("textures/box2/box2-diffus-mask.png").c_str());
    Texture* box2_specular = Texture::load(std::string(_resources_directory).append("textures/box2/box2-specular.png").c_str());
    Texture* box2_specular_albedo = Texture::load(std::string(_resources_directory).append("textures/box2/box2-specular-albedo.png").c_str());
    Texture* box2_specular_mask = Texture::load(std::string(_resources_directory).append("textures/box2/box2-specular-mask.png").c_str());

    Mesh * dragon_small = Mesh::load(std::string(_resources_directory).append("meshes/dragon2_small.obj").c_str());
    scene.addMesh(dragon_small);
    dragon_small->setScale(glm::vec3(2.f, 2.f, 2.f));
    dragon_small->setPosition(glm::vec3(3.f, 0, 0));

    AmbiantLight* ambiantLight = new AmbiantLight();
    scene.addAmbiantLight(ambiantLight);
    DirectionalLight* directionalLight = new DirectionalLight();
    scene.addDirectionalLight(directionalLight);

    int width, height;
    double mouseX = 0, mouseY = 0, mouseX_old = 0, mouseY_old = 0, mouseSpeedX = 0, mouseSpeedY = 0;

    Object * selectedObject = nullptr;

    // create a file browser instance
    ImGui::FileBrowser fileDialog;

    // (optional) set browser properties
    fileDialog.SetTitle("Charger un objet");
    fileDialog.SetTypeFilters({ ".obj" });

    double start = glfwGetTime();
    while(!glfwWindowShouldClose(window.getGFLWwindow()))
    {
        // FPS
        double end = glfwGetTime();
        double frameTime = end - start;
        start = (float)glfwGetTime();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        float windowWidth = 400;
        float windowHeight = float(height);

        ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        if (ImGui::Begin("Window", nullptr, ImGuiWindowFlags_NoResize)) {

            ImGui::TextColored(ImVec4(0,1,0,1), std::string("FPS : " + std::to_string(int(1.f/frameTime))).c_str());
            ImGui::TextColored(ImVec4(0,1,0,1), std::string("Frame Time : " + std::to_string(frameTime*1000) + "ms").c_str());

            if (ImGui::BeginTabBar("Tabs")) {

                if (ImGui::BeginTabItem("Scene")) {

                    if (ImGui::Button("Charger un objet")) {
                        fileDialog.Open();
                    }

                    fileDialog.Display();

                    if (fileDialog.HasSelected())
                    {
                        std::string path = fileDialog.GetSelected().string();
                        fileDialog.ClearSelected();
                        Mesh * object = Mesh::load(path.c_str());
                        scene.addMesh(object);
                        selectedObject = object;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Ajouter un cube")) {
                        Mesh * mesh = new Cube();
                        scene.addMesh(mesh);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Ajouter un disque")) {
                        Mesh * mesh = new Disk();
                        scene.addMesh(mesh);
                    }

                    if (ImGui::Button("Ajouter un plan")) {
                        Mesh * mesh = new Square();
                        scene.addMesh(mesh);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Ajouter une lumière ambiante")) {
                        AmbiantLight * light = new AmbiantLight();
                        scene.addAmbiantLight(light);
                    }

                    if (ImGui::Button("Ajouter une lumière directionnelle")) {
                        DirectionalLight * light = new DirectionalLight();
                        scene.addDirectionalLight(light);
                    }

                    if (ImGui::Button("Ajouter une lumière ponctuelle")) {
                        PointLight * light = new PointLight();
                        scene.addPointLight(light);
                    }

                    ImGui::TextColored(ImVec4(1,1,0,1), "Objets");
                    if (ImGui::BeginChild("Mesh list", ImVec2(windowWidth, 200))) {

                        if (ImGui::Selectable("Camera", scene.getCamera() == selectedObject)) {
                            selectedObject = scene.getCamera();
                        }

                        for (long long unsigned int i = 0; i < scene.getAmbiantLights().size(); ++i) {
                            std::string name = "Ambiant light " + std::to_string(i);
                            if (ImGui::Selectable(name.c_str(), scene.getAmbiantLights().at(i) == selectedObject)) {
                                selectedObject = scene.getAmbiantLights().at(i);
                            }
                        }

                        for (long long unsigned int i = 0; i < scene.getDirectionalLights().size(); ++i) {
                            std::string name = "Directional light " + std::to_string(i);
                            if (ImGui::Selectable(name.c_str(), scene.getDirectionalLights().at(i) == selectedObject)) {
                                selectedObject = scene.getDirectionalLights().at(i);
                            }
                        }

                        for (long long unsigned int i = 0; i < scene.getPointLights().size(); ++i) {
                            std::string name = "Point light " + std::to_string(i);
                            if (ImGui::Selectable(name.c_str(), scene.getPointLights().at(i) == selectedObject)) {
                                selectedObject = scene.getPointLights().at(i);
                            }
                        }

                        for (long long unsigned int i = 0; i < scene.getMeshes().size(); ++i) {
                            std::string name = "Mesh " + std::to_string(i);
                            if (ImGui::Selectable(name.c_str(), scene.getMeshes().at(i) == selectedObject)) {
                                selectedObject = scene.getMeshes().at(i);
                            }
                        }

                    } ImGui::EndChild();

                    ImGui::TextColored(ImVec4(1,1,0,1), "Propriétés");
                    if (ImGui::BeginChild("Properties list", ImVec2(windowWidth, float(height)-450)))
                    {
                        if (selectedObject == nullptr) {
                            ImGui::Text("Aucun objet sélectionné");
                        } else {
                            float position[3] = {selectedObject->getPosition().x, selectedObject->getPosition().y, selectedObject->getPosition().z};
                            float scale[3] = {selectedObject->getScale().x, selectedObject->getScale().y, selectedObject->getScale().z};
                            float rotation[3] = {selectedObject->getRotation().x, selectedObject->getRotation().y, selectedObject->getRotation().z};
                            ImGui::DragFloat3("Position", position, 0.1, -100, 100);
                            ImGui::DragFloat3("Scale", scale, 0.05, -100, 100);
                            ImGui::DragFloat3("Rotation", rotation, 0.5, -360, 360);
                            selectedObject->setPosition(glm::vec3(position[0], position[1], position[2]));
                            selectedObject->setScale(glm::vec3(scale[0], scale[1], scale[2]));
                            selectedObject->setRotation(glm::vec3(rotation[0], rotation[1], rotation[2]));

                            Mesh* mesh = dynamic_cast<Mesh*>(selectedObject);
                            Light* light = dynamic_cast<Light*>(selectedObject);

                            if (mesh != nullptr) {
                                ImGui::TextColored(ImVec4(1,1,0,1), "Propriétés du mesh");

                                ImGui::Text("Shader");

                                if (ImGui::BeginChild("Shaders", ImVec2(windowWidth, 100))) {
                                    std::map<std::string, Shader*> shaderList = shaderManager.getShaders();
                                    for (auto it = shaderList.begin(); it != shaderList.end() ; ++it) {
                                        if (ImGui::Button(it->first.c_str())) {
                                            mesh->setShader(it->second);
                                        }
                                    }
                                } ImGui::EndChild();

                                if (ImGui::Button("Supprimer")) {
                                    scene.removeMesh(mesh);
                                    selectedObject = nullptr;
                                }
                            }

                            if (light != nullptr) {
                                ImGui::TextColored(ImVec4(1,1,0,1), "Propriétés de la lumière");
                                float color[3] = {light->getColor().r,light->getColor().g,light->getColor().b};
                                ImGui::ColorEdit3("Couleur", color);
                                light->setColor(glm::vec3(color[0],color[1],color[2]));

                                AmbiantLight* ambiantLight = dynamic_cast<AmbiantLight*>(light);
                                DirectionalLight* directionalLight = dynamic_cast<DirectionalLight*>(light);
                                PointLight* pointLight = dynamic_cast<PointLight*>(light);

                                if (pointLight != nullptr) {
                                    float constant = pointLight->getConstant();
                                    float linear = pointLight->getLinear();
                                    float quadratic = pointLight->getQuadratic();
                                    ImGui::DragFloat("Constant", &constant);
                                    ImGui::DragFloat("Linear", &linear);
                                    ImGui::DragFloat("Quadratic", &quadratic);
                                    pointLight->setConstant(constant);
                                    pointLight->setLinear(linear);
                                    pointLight->setQuadratic(quadratic);
                                }

                                if (ImGui::Button("Supprimer")) {
                                    if (ambiantLight != nullptr) scene.removeAmbiantLight(ambiantLight);
                                    else if (directionalLight != nullptr) scene.removeDirectionalLight(directionalLight);
                                    else if (pointLight != nullptr) scene.removePointLight(pointLight);
                                    selectedObject = nullptr;
                                }
                            }
                        }

                    } ImGui::EndChild();

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Paramètres")) {
                    float backgroundColor[4] = {
                            renderer.getBackgroundColor()[0],
                            renderer.getBackgroundColor()[1],
                            renderer.getBackgroundColor()[2],
                            renderer.getBackgroundColor()[3]
                    };
                    ImGui::ColorEdit4("Couleur de fond", backgroundColor);
                    renderer.setBackgroundColor(glm::vec4(backgroundColor[0],backgroundColor[1],backgroundColor[2],backgroundColor[3]));
                    float gamma = renderer.getGamma();
                    ImGui::DragFloat("Gamma", &gamma, 0.1,1,5);
                    renderer.setGamma(gamma);
                    float exposition = renderer.getExposition();
                    ImGui::DragFloat("Exposition", &exposition, 0.1,0,5);
                    renderer.setExposition(exposition);
                    float bloom = renderer.getBloom();
                    ImGui::DragFloat("Bloom", &bloom, 0.1,0,5);
                    renderer.setBloom(bloom);
                    ImGui::EndTabItem();
                    bool renderOutline = renderer.getRenderOutline();
                    ImGui::Checkbox("Rendu des contours", &renderOutline);
                    renderer.setRenderOutline(renderOutline);
                }

                ImGui::EndTabBar();
            }

        }
        ImGui::End();

        ImGuiIO& io = ImGui::GetIO();

        glfwGetWindowSize(window.getGFLWwindow(), &width, &height);
        glfwGetCursorPos(window.getGFLWwindow(), &mouseX, &mouseY);
        mouseSpeedX = mouseX - mouseX_old;
        mouseSpeedY = mouseY - mouseY_old;
        mouseX_old = mouseX;
        mouseY_old = mouseY;

        scene.update(frameTime);
        float camX = 0, camY = 0, camZ = 0;
        float camSpeed = 5;
        float mouseSens = 0.2;

        glm::vec3 forward = scene.getCamera()->getForwardVector();
        glm::vec3 right = scene.getCamera()->getRightVector();
        glm::vec3 up = scene.getCamera()->getUpVector();

        if (!io.WantCaptureMouse)
        {
            if (window.isPressingMouseButton(GLFW_MOUSE_BUTTON_LEFT) ||
                window.isPressingMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) {
                glfwSetInputMode(window.getGFLWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

                if (window.isPressingKey(GLFW_KEY_UP))
                    scene.getCamera()->setRotation(
                            scene.getCamera()->getRotation() + glm::vec3(1 * frameTime * -100, 0, 0));
                if (window.isPressingKey(GLFW_KEY_DOWN))
                    scene.getCamera()->setRotation(scene.getCamera()->getRotation() + glm::vec3(1 * frameTime * 100, 0, 0));
                if (window.isPressingKey(GLFW_KEY_LEFT))
                    scene.getCamera()->setRotation(
                            scene.getCamera()->getRotation() + glm::vec3(0, 1 * frameTime * -100, 0));
                if (window.isPressingKey(GLFW_KEY_RIGHT))
                    scene.getCamera()->setRotation(scene.getCamera()->getRotation() + glm::vec3(0, 1 * frameTime * 100, 0));

                if (window.isPressingKey(GLFW_KEY_W))
                    camZ += 1;
                if (window.isPressingKey(GLFW_KEY_S))
                    camZ += -1;
                if (window.isPressingKey(GLFW_KEY_A))
                    camX += -1;
                if (window.isPressingKey(GLFW_KEY_D))
                    camX += 1;
                if (window.isPressingKey(GLFW_KEY_Q))
                    camY += -1;
                if (window.isPressingKey(GLFW_KEY_E))
                    camY += 1;

                scene.getCamera()->setPosition(
                        scene.getCamera()->getPosition()
                        + glm::vec3(forward.x * camZ * frameTime * camSpeed, forward.y * camZ * frameTime * camSpeed,
                                    forward.z * camZ * frameTime * camSpeed)
                        + glm::vec3(right.x * camX * frameTime * camSpeed, right.y * camX * frameTime * camSpeed,
                                    right.z * camX * frameTime * camSpeed)
                        + glm::vec3(up.x * camY * frameTime * camSpeed, up.y * camY * frameTime * camSpeed,
                                    up.z * camY * frameTime * camSpeed)
                );

                if (window.isPressingMouseButton(GLFW_MOUSE_BUTTON_RIGHT) &&
                    window.isPressingMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
                    scene.getCamera()->setPosition(scene.getCamera()->getPosition()
                                                   + glm::vec3(up.x * mouseSpeedY * -0.001 * camSpeed,
                                                               up.y * mouseSpeedY * -0.001 * camSpeed,
                                                               up.z * mouseSpeedY * -0.001 * camSpeed)
                                                   + glm::vec3(right.x * mouseSpeedX * 0.001 * camSpeed,
                                                               right.y * mouseSpeedX * 0.001 * camSpeed,
                                                               right.z * mouseSpeedX * 0.001 * camSpeed)
                    );
                } else if (window.isPressingMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) {
                    scene.getCamera()->setRotation(scene.getCamera()->getRotation() +
                                                   glm::vec3(mouseSpeedY * mouseSens, mouseSpeedX * mouseSens, 0));
                } else if (window.isPressingMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
                    scene.getCamera()->setPosition(scene.getCamera()->getPosition()
                                                   + glm::vec3(forward.x * mouseSpeedY * -0.001 * camSpeed,
                                                               forward.y * mouseSpeedY * -0.001 * camSpeed,
                                                               forward.z * mouseSpeedY * -0.001 * camSpeed)
                    );
                    scene.getCamera()->setRotation(scene.getCamera()->getRotation()
                                                   + glm::vec3(0, mouseSpeedX * mouseSens, 0)
                    );
                }

            } else {
                glfwSetInputMode(window.getGFLWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }

        textureShader->use();
        textureShader->setTexture("box2", box2->getTexture(), 0);
        textureShader->setTexture("box2_diffus", box2_diffus->getTexture(), 1);
        textureShader->setTexture("box2_diffus_mask", box2_diffus_mask->getTexture(), 2);
        textureShader->setTexture("box2_specular", box2_specular->getTexture(), 3);
        textureShader->setTexture("box2_specular_albedo", box2_specular_albedo->getTexture(), 4);
        textureShader->setTexture("box2_specular_mask", box2_specular_mask->getTexture(), 5);

        renderer.setScreenSize(width, height);
        renderer.render(&scene);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window.getGFLWwindow());
        glfwPollEvents();
    }

    return 0;
}


