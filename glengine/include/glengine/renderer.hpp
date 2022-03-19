#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "scene.hpp"

namespace GLEngine {

    class Renderer {

    private:
        Shader* scaleShader;
        int width, height;
        float* backgroundColor;
        bool renderOutline;
        unsigned int framebuffer;
        unsigned int screenVAO;

    public:
        Renderer();
        ~Renderer();
        void render(Scene* scene) const;

        inline void setScreenSize(int _width, int _height) {width = _width; height = _height;}
        inline float* getBackgroundColor() {return backgroundColor;}
        inline void setBackgroundColor(float* _backgroundColor) {backgroundColor = _backgroundColor;}
    };

}

#endif