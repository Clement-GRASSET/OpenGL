#include <glengine/window.hpp>
#include <iostream>
#include <string>
#include "include/imgui/imgui.h"
#include "include/imgui/imgui_impl_glfw.h"
#include "include/imgui/imgui_impl_opengl3.h"

namespace GLEngine {

    void Window::framebuffer_size_callback(GLFWwindow* win, int w, int h)
    {
        glViewport(0, 0, w, h);
    }

    Window::Window(int width, int height)
    : win(nullptr)
    {
        // ça part de là !
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        win = glfwCreateWindow(int(width), int(height), "TP1", NULL, NULL);
        if (win == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
        glfwMakeContextCurrent(win);
        glfwSwapInterval(0);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            throw std::runtime_error("Failed to initialize GLAD");
        }

        glViewport(0, 0, int(width), int(height));

        glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO & io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(win, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    Window::~Window()
    {
        glfwTerminate();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    bool Window::isPressingKey(int key) {
        return glfwGetKey(win, key) == GLFW_PRESS;
    }

    bool Window::isPressingMouseButton(int mouseButton) {
        return glfwGetMouseButton(win, mouseButton) == GLFW_PRESS;
    }

}