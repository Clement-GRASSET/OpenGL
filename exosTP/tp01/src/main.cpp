#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glengine/square.hpp>
#include "glengine/disc.hpp"
#include "glengine/terrain.hpp"
#include <glengine/cube.hpp>
#include "glengine/scene.hpp"
#include "glengine/renderer.hpp"
#include "stbimage/stb_image.h"
#include "glengine/window.hpp"
#include "tp01/customScene.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

using namespace GLEngine;

extern const char* _resources_directory;

unsigned int loadTexture(const char* path, GLint colors)
{
    //glActiveTexture(GL_TEXTURE0);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        /*for (size_t i = 0; i < width*height*nrChannels; ++i) {
            std::cout << (unsigned int)(data[i]) << ", ";
        }*/
        glTexImage2D(GL_TEXTURE_2D, 0, colors, width, height, 0, colors, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture : " << path << std::endl;
    }
    stbi_image_free(data);
    return texture;
}

int main() {

    Window window(1280, 720);
    CustomScene scene;
    Renderer renderer;

    /*
    Mesh * square = new Square();
    scene.addMesh(square);
    square->setPosition(glm::vec3(1.f, 0.f, 0.f));
    square->setScale(glm::vec3(0.5f, 0.5f, 0.5f));

    Mesh * terrain = new Terrain();
    scene.addMesh(terrain);
    terrain->setPosition(glm::vec3(0.f, 0.f, 0.f));
    terrain->setScale(glm::vec3(0.5f, 0.5f, 0.5f));

    Mesh * disk = new Disk();
    scene.addMesh(disk);
    disk->setPosition(glm::vec3(0.f, 0.f, 0.f));
    disk->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
    */

    Mesh * cube = new Cube();
    scene.addMesh(cube);
    cube->setPosition(glm::vec3(0.f, 0.f, 0.f));
    //cube->setScale(glm::vec3(2.f, 2.f, 2.f));

    unsigned int box2 = loadTexture(std::string(_resources_directory).append("textures/box2/box2.jpg").c_str(), GL_RGB);
    unsigned int box2_diffus = loadTexture(std::string(_resources_directory).append("textures/box2/box2-diffus.png").c_str(), GL_RGB);
    unsigned int box2_diffus_mask = loadTexture(std::string(_resources_directory).append("textures/box2/box2-diffus-mask.png").c_str(), GL_RGBA);
    unsigned int box2_specular = loadTexture(std::string(_resources_directory).append("textures/box2/box2-specular.png").c_str(), GL_RGBA);
    unsigned int box2_specular_albedo = loadTexture(std::string(_resources_directory).append("textures/box2/box2-specular-albedo.png").c_str(), GL_RGBA);
    unsigned int box2_specular_mask = loadTexture(std::string(_resources_directory).append("textures/box2/box2-specular-mask.png").c_str(), GL_RGBA);

    cube->getShader()->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, box2);
    cube->getShader()->setInt("box2", 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, box2_diffus);
    cube->getShader()->setInt("box2_diffus", 1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, box2_diffus_mask);
    cube->getShader()->setInt("box2_diffus_mask", 2);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, box2_specular);
    cube->getShader()->setInt("box2_specular", 3);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, box2_specular_albedo);
    cube->getShader()->setInt("box2_specular_albedo", 4);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, box2_specular_mask);
    cube->getShader()->setInt("box2_specular_mask", 5);

    Mesh * bunny = Mesh::load(std::string(_resources_directory).append("meshes/bunny.obj").c_str());
    scene.addMesh(bunny);
    bunny->setPosition(glm::vec3(1.8f, 0.1f, 0));
    bunny->setScale(glm::vec3(0.5f, 0.5f, 0.5f));

    Mesh * dragon_small = Mesh::load(std::string(_resources_directory).append("meshes/dragon2_small.obj").c_str());
    scene.addMesh(dragon_small);
    dragon_small->setScale(glm::vec3(2.f, 2.f, 2.f));
    dragon_small->setPosition(glm::vec3(3.f, 0, 0));

    Mesh * dragon_smooth = Mesh::load(std::string(_resources_directory).append("meshes/dragon2_smooth.obj").c_str());
    scene.addMesh(dragon_smooth);
    dragon_smooth->setPosition(glm::vec3(5.f, -0.5f, 0.f));
    dragon_smooth->setScale(glm::vec3(2.f, 2.f, 2.f));

    int width, height;
    double mouseX = 0, mouseY = 0, mouseX_old = 0, mouseY_old = 0, mouseSpeedX = 0, mouseSpeedY = 0;

    double start = glfwGetTime();
    double timeBeforeRefresh = 0;
    while(!glfwWindowShouldClose(window.getGFLWwindow()))
    {
        // FPS
        double end = glfwGetTime();
        double frameTime = end - start;
        start = (float)glfwGetTime();

        timeBeforeRefresh -= frameTime;
        if (timeBeforeRefresh <= 0) {
            std::string title = "FPS : " + std::to_string(int(1.f/frameTime)) + ", frame time : " + std::to_string(frameTime) + "ms";
            glfwSetWindowTitle(window.getGFLWwindow(), title.c_str());
            timeBeforeRefresh = 0.2;
        }

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

        if (window.isPressingMouseButton(GLFW_MOUSE_BUTTON_LEFT) || window.isPressingMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) {
            glfwSetInputMode(window.getGFLWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            if (window.isPressingMouseButton(GLFW_MOUSE_BUTTON_RIGHT) && window.isPressingMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
                scene.getCamera()->setPosition(scene.getCamera()->getPosition()
                    + glm::vec3(up.x*mouseSpeedY*-0.001*camSpeed, up.y*mouseSpeedY*-0.001*camSpeed, up.z*mouseSpeedY*-0.001*camSpeed)
                    + glm::vec3(right.x*mouseSpeedX*0.001*camSpeed, right.y*mouseSpeedX*0.001*camSpeed, right.z*mouseSpeedX*0.001*camSpeed)
                );
            } else if (window.isPressingMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) {
                scene.getCamera()->setRotation(scene.getCamera()->getRotation() + glm::vec3(mouseSpeedY*mouseSens, mouseSpeedX*mouseSens, 0));
            } else if (window.isPressingMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
                scene.getCamera()->setPosition(scene.getCamera()->getPosition()
                   + glm::vec3(forward.x*mouseSpeedY*-0.001*camSpeed, forward.y*mouseSpeedY*-0.001*camSpeed, forward.z*mouseSpeedY*-0.001*camSpeed)
                );
                scene.getCamera()->setRotation(scene.getCamera()->getRotation()
                   + glm::vec3(0, mouseSpeedX*mouseSens, 0)
                );
            }

        } else {
            glfwSetInputMode(window.getGFLWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }



        if (window.isPressingKey(GLFW_KEY_UP))
            scene.getCamera()->setRotation(scene.getCamera()->getRotation() + glm::vec3(1*frameTime*-100,0,0));
        if (window.isPressingKey(GLFW_KEY_DOWN))
            scene.getCamera()->setRotation(scene.getCamera()->getRotation() + glm::vec3(1*frameTime*100,0,0));
        if (window.isPressingKey(GLFW_KEY_LEFT))
            scene.getCamera()->setRotation(scene.getCamera()->getRotation() + glm::vec3(0,1*frameTime*-100,0));
        if (window.isPressingKey(GLFW_KEY_RIGHT))
            scene.getCamera()->setRotation(scene.getCamera()->getRotation() + glm::vec3(0, 1*frameTime*100,0));

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
            + glm::vec3(forward.x*camZ*frameTime*camSpeed,forward.y*camZ*frameTime*camSpeed,forward.z*camZ*frameTime*camSpeed)
            + glm::vec3(right.x*camX*frameTime*camSpeed,right.y*camX*frameTime*camSpeed,right.z*camX*frameTime*camSpeed)
            + glm::vec3(up.x*camY*frameTime*camSpeed,up.y*camY*frameTime*camSpeed,up.z*camY*frameTime*camSpeed)
        );

        renderer.setScreenSize(width, height);
        renderer.render(&scene);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(300.0f, float(height)));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        if (ImGui::Begin("Window", nullptr, ImGuiWindowFlags_NoResize)) {

            if (ImGui::BeginTabBar("Tabs")) {

                if (ImGui::BeginTabItem("Scene")) {

                    if (ImGui::Button("Charger un objet")) {

                    }

                    ImGui::TextColored(ImVec4(1,1,0,1), "Objets");
                    ImGui::BeginChild("Scrolling", ImVec2(100, 100));
                    for (long long unsigned int i = 0; i < scene.getMeshes().size(); ++i) {
                        //Mesh* m = scene.getMeshes().at(i);
                        std::string name = "Mesh " + std::to_string(i);
                        ImGui::Text(name.c_str());

                    }
                    ImGui::EndChild();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Paramètres")) {
                    ImGui::ColorEdit4("Couleur de fond", renderer.getBackgroundColor());
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window.getGFLWwindow());
        glfwPollEvents();
    }

    return 0;
}
