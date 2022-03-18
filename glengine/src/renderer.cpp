#include "glengine/renderer.hpp"

namespace GLEngine {

    Renderer::Renderer()
    : width(1), height(1)
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

        renderOutline = true;

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Pour passer en wireframe
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
    }

    Renderer::~Renderer()
    {
        delete [] backgroundColor;
        delete scaleShader;
    }

    void Renderer::render(Scene *scene) const
    {
        Camera* camera = scene->getCamera();

        glm::mat4 view = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
        view = glm::rotate(view, glm::radians(scene->getCamera()->getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, glm::radians(scene->getCamera()->getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::rotate(view, glm::radians(scene->getCamera()->getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
        view = glm::translate(view, -camera->getPosition());
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

            m->getShader()->use();
            m->getShader()->setVec3("viewPos", camera->getPosition());
            m->getShader()->setInt("width", int(width));
            m->getShader()->setInt("height", int(height));
            m->render(view, projection, m->getShader());

            if (renderOutline) {
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

}