#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "scene.hpp"

namespace GLEngine {

    class Renderer {

    private:
        Shader* scaleShader;
        Shader* backgroundShader;
        Shader* screenShader;
        Shader* blurShader;
        int width, height;
        float* backgroundColor;
        float gamma;
        bool renderOutline;
        unsigned int framebuffer, textureColorBuffer, textureBloomBuffer, rbo;
        unsigned int pingpongFBO[2];
        unsigned int pingpongBuffer[2];
        unsigned int screenVAO, screenVBO, screenEBO;

        void generateScreenVAO();
        void generateBuffers();
        unsigned int generateTexture() const;
        void RenderQuad() const;

    public:
        Renderer();
        ~Renderer();
        void render(Scene* scene) const;
        void setScreenSize(int _width, int _height);

        inline float* getBackgroundColor() {return backgroundColor;}
        inline void setBackgroundColor(float* _backgroundColor) {backgroundColor = _backgroundColor;}
    };

}

#endif