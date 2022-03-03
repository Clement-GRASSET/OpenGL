#include "glengine/renderer.hpp"

namespace GLEngine {

    Renderer::Renderer()
    {
        backgroundColor = new float[4];
        backgroundColor[0] = 0.3f;
        backgroundColor[1] = 0.3f;
        backgroundColor[2] = 0.3f;
        backgroundColor[3] = 1.f;

        scaleShader = new Shader(
                "C:/Users/Clement/Documents/IUT/Projets/OpenGL/glengine/resources/shaders/scale.vert",
                "C:/Users/Clement/Documents/IUT/Projets/OpenGL/glengine/resources/shaders/scale.frag"
        );
    }

    Renderer::~Renderer()
    {
        delete [] backgroundColor;
        delete scaleShader;
    }

    void Renderer::render(Scene *scene) const
    {
        glm::mat4 view = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        view = glm::rotate(view, glm::radians(scene->getCamera()->getRotation()[0]), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, glm::radians(scene->getCamera()->getRotation()[1]), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.f), float(width)/float(height), 0.1f, 100.0f);

        // Draw
        glEnable(GL_DEPTH_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        for (auto & m : scene->getMeshes()) {

            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);

            m->render(view, projection, m->getShader());

            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00);
            glDisable(GL_DEPTH_TEST);

            m->render(view, projection, scaleShader);

            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glEnable(GL_DEPTH_TEST);
        }
    }

}