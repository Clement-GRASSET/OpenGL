#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glengine/shader.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "tp01/config.hpp"

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


    std::vector<float> vertices = {
            // position            // colors
            -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,

            -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,

            0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
            0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,

            -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
            0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
    };

    Shader shader = Shader(
            std::string(_resources_directory).append("shader/simple.vert").c_str(),
            std::string(_resources_directory).append("shader/simple.frag").c_str()
            );

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof (float)));
    glEnableVertexAttribArray(1);

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

        rotation += float(frameTime*30.0f);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 view = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
        view = glm::rotate(glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)), glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.f), float(w)/float(h), 0.1f, 100.0f);

        shader.setMat4fv("model", model);
        shader.setMat4fv("view", view);
        shader.setMat4fv("projection", projection);

        glEnable(GL_DEPTH_TEST);


        // Draw
        glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();


    }

    glfwTerminate();

    return 0;
}
