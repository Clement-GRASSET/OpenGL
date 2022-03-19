#include <glengine/renderer.hpp>

extern const char* _resources_directory;

namespace GLEngine {

    Renderer::Renderer()
    : width(1), height(1), renderOutline(true), framebuffer(0), screenVAO(0)
    {
        backgroundColor = new float[4];
        backgroundColor[0] = 0.3f;
        backgroundColor[1] = 0.3f;
        backgroundColor[2] = 0.3f;
        backgroundColor[3] = 1.f;

        scaleShader = new Shader(
                std::string(_resources_directory).append("shaders/scale.vert").c_str(),
                std::string(_resources_directory).append("shaders/scale.frag").c_str()
        );

        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        // generate texture
        unsigned int textureColorbuffer;
        glGenTextures(1, &textureColorbuffer);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        // attach it to currently bound framebuffer object
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Pour passer en wireframe
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
    }

    Renderer::~Renderer()
    {
        delete [] backgroundColor;
        delete scaleShader;
        glDeleteFramebuffers(1, &framebuffer);
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
        //glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

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

        //glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);
    }

}