#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glengine/shader.hpp>
//#include <glengine/mesh.hpp>
#include <glengine/square.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glengine/disc.hpp"
#include "glengine/terrain.hpp"
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

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Pour passer en wireframe
    glEnable(GL_DEPTH_TEST);

    std::vector<Mesh*> meshes;

    meshes.push_back(new Square());
    meshes.at(0)->setPosition(glm::vec3(0.f, 1.f, 0.f));
    meshes.at(0)->setScale(glm::vec3(0.5f, 0.5f, 0.5f));

    meshes.push_back(new Terrain());
    meshes.at(1)->setPosition(glm::vec3(0.f, 0.f, 0.f));
    meshes.at(1)->setScale(glm::vec3(0.5f, 0.5f, 0.5f));

    meshes.push_back(new Disk());
    meshes.at(2)->setPosition(glm::vec3(0.f, 0.f, 0.f));
    meshes.at(2)->setScale(glm::vec3(0.5f, 0.5f, 0.5f));

    double start = glfwGetTime();
    float rotation = 0;
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

        rotation += float(frameTime*100.0f);

        glm::mat4 view = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        view = glm::rotate(view, glm::radians(30.f), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.f), float(w)/float(h), 0.1f, 100.0f);

        // Draw
        glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (size_t i = 0; i < meshes.size(); i++) {
            meshes.at(i)->render(view, projection);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
