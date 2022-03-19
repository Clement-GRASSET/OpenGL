#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace GLEngine {

    class Window {

    private:
        GLFWwindow* win;

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    public:
        Window(int width, int height);
        ~Window();
        GLFWwindow* getGFLWwindow() {return win;};
        //void setSize(unsigned int _width, unsigned int _height) {width = _width; height = _height;};
        bool isPressingKey(int key);
        bool isPressingMouseButton(int mouseButton);

    };

}

#endif