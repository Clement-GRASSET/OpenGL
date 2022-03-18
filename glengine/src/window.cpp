#include <glengine/window.hpp>
#include <iostream>
#include <string>

namespace GLEngine {

    void Window::framebuffer_size_callback(GLFWwindow* win, int w, int h)
    {
        glViewport(0, 0, w, h);
    }

    Window::Window(int width, int height)
    : window(nullptr)
    {
        // ça part de là !
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        window = glfwCreateWindow(int(width), int(height), "TP1", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(0);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            throw std::runtime_error("Failed to initialize GLAD");
        }

        glViewport(0, 0, int(width), int(height));

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        //while(!glfwWindowShouldClose(window)) {
        //    glfwGetWindowSize(window, &width, &height);
            //if (refreshCallback != nullptr) {
            //    (*refreshCallback)();
            //}
        //}
    }

    Window::~Window()
    {
        glfwTerminate();
        delete window;
    }

    bool Window::isPressingKey(int key) {
        return glfwGetKey(window, key) == GLFW_PRESS;
    }

}