#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <glengine/square.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glengine/disc.hpp"
#include "glengine/terrain.hpp"
#include "glengine/scene.hpp"
#include "glengine/renderer.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
//#include "tp01/config.hpp"

using namespace GLEngine;

int w = 800, h = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    w = width;
    h = height;
    glViewport(0, 0, w, h);
}

int main() {
    // ça part de là !
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(w, h, "TP1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, w, h);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Pour passer en wireframe
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO & io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Scene scene;
    Renderer renderer;

    Mesh * square = new Square();
    //scene.addMesh(square);
    square->setPosition(glm::vec3(0.f, 1.f, 0.f));
    square->setScale(glm::vec3(0.5f, 5.f, 0.5f));

    Mesh * terrain = new Terrain();
    //scene.addMesh(terrain);
    terrain->setPosition(glm::vec3(0.f, 0.f, 0.f));
    terrain->setScale(glm::vec3(0.5f, 0.5f, 0.5f));

    Mesh * disk = new Disk;
    //scene.addMesh(disk);
    disk->setPosition(glm::vec3(0.f, 0.f, 0.f));
    disk->setScale(glm::vec3(0.5f, 0.5f, 0.5f));

    Mesh * bunny = Mesh::load("C:/Users/Clement/Documents/IUT/Projets/OpenGL/glengine/resources/meshes/bunny.obj");
    scene.addMesh(bunny);
    bunny->setPosition(glm::vec3(1.8f, 0.1f, 0));
    bunny->setScale(glm::vec3(0.5f, 0.5f, 0.5f));

    //Mesh * dragon_small = Mesh::load("C:/Users/Clement/Documents/IUT/Projets/OpenGL/glengine/resources/meshes/dragon2_small.obj");
    //scene.addMesh(dragon_small);
    //dragon_small->setScale(glm::vec3(2.f, 0.5f, 2.f));

    Mesh * dragon_smooth = Mesh::load("C:/Users/Clement/Documents/IUT/Projets/OpenGL/glengine/resources/meshes/dragon2_smooth.obj");
    scene.addMesh(dragon_smooth);
    dragon_smooth->setPosition(glm::vec3(0.f, -0.5f, 0.f));
    //dragon_smooth->setScale(glm::vec3(2.f, 2.f, 2.f));

    double start = glfwGetTime();
    double timeBeforeRefresh = 0;
    while(!glfwWindowShouldClose(window))
    {
        // FPS
        double end = glfwGetTime();
        double frameTime = end - start;
        start = (float)glfwGetTime();

        timeBeforeRefresh -= frameTime;
        if (timeBeforeRefresh <= 0) {
            std::string title = "FPS : " + std::to_string(int(1.f/frameTime)) + ", frame time : " + std::to_string(frameTime) + "ms";
            glfwSetWindowTitle(window, title.c_str());
            timeBeforeRefresh = 0.5;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        scene.update(frameTime);
        renderer.setScreenSize(w, h);
        renderer.render(&scene);

        ImGui::SetNextWindowSize(ImVec2(300.0f, float(h)));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        if (ImGui::Begin("Window", nullptr, ImGuiWindowFlags_NoResize)) {

            if (ImGui::BeginTabBar("Tabs")) {

                if (ImGui::BeginTabItem("Scene")) {
                    ImGui::TextColored(ImVec4(1,1,0,1), "Objets");
                    ImGui::BeginChild("Scrolling", ImVec2(100, 100));
                    for (long long unsigned int i = 0; i < scene.getMeshes().size(); ++i) {
                        //Mesh* m = scene.getMeshes().at(i);
                        std::string name = "Mesh " + std::to_string(i);
                        ImGui::Text(name.c_str());
                    }
                    ImGui::EndChild();
                } //ImGui::EndTabItem();

                if (ImGui::BeginTabItem("Parametres")) {
                    ImGui::ColorEdit4("Couleur de fond", renderer.getBackgroundColor());
                } ImGui::EndTabItem();

            } ImGui::EndTabBar();

        } ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return 0;
}
