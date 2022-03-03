#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "scene.hpp"

namespace GLEngine {

    class Renderer {

    private:
        Shader* scaleShader;
        unsigned int width, height;
        float* backgroundColor;

    public:
        Renderer();
        ~Renderer();
        void render(Scene* scene) const;

        inline void setScreenSize(unsigned int _width, unsigned int _height) {width = _width; height = _height;}
        inline float* getBackgroundColor() {return backgroundColor;}
        inline void setBackgroundColor(float* _backgroundColor) {backgroundColor = _backgroundColor;}
    };

}

#endif