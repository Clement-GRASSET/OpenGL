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
        glm::vec4 backgroundColor;
        float gamma, exposition, bloom;
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

        inline glm::vec4 getBackgroundColor() {return backgroundColor;}
        inline void setBackgroundColor(glm::vec4 _backgroundColor) {backgroundColor = _backgroundColor;}

        inline float getGamma() const {return gamma;}
        inline void setGamma(float _gamma) {gamma = _gamma;}

        inline float getExposition() const {return exposition;}
        inline void setExposition(float _exposition) {exposition = _exposition;}

        inline float getBloom() const {return bloom;}
        inline void setBloom(float _bloom) {bloom = _bloom;}
    };

}

#endif