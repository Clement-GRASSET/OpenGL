#include <glengine/renderer.hpp>

extern const char* _resources_directory;

namespace GLEngine {

    Renderer::Renderer()
    : width(1), height(1), backgroundColor(glm::vec4(0.1,0.1,0.1,1.0)), gamma(2.2f), exposition(1.f), bloom(1.f), renderOutline(false), framebuffer(0), textureColorBuffer(0), textureBloomBuffer(0), rbo(0), screenVAO(0), screenVBO(0), screenEBO(0)
    {
        scaleShader = new Shader(
                std::string(_resources_directory).append("shaders/scale.vert").c_str(),
                std::string(_resources_directory).append("shaders/scale.frag").c_str()
        );

        backgroundShader = new Shader(
                std::string(_resources_directory).append("shaders/screen.vert").c_str(),
                std::string(_resources_directory).append("shaders/background.frag").c_str()
        );

        screenShader = new Shader(
                std::string(_resources_directory).append("shaders/screen.vert").c_str(),
                std::string(_resources_directory).append("shaders/screen.frag").c_str()
        );

        blurShader = new Shader(
                std::string(_resources_directory).append("shaders/screen.vert").c_str(),
                std::string(_resources_directory).append("shaders/blur.frag").c_str()
        );

        generateScreenVAO();
        generateBuffers();

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Pour passer en wireframe
        //glEnable(GL_DEPTH_TEST);
        //glEnable(GL_STENCIL_TEST);
    }

    Renderer::~Renderer()
    {
        delete scaleShader;
        delete screenShader;
        delete blurShader;
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
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glViewport(0,0,width,height);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        //glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        backgroundShader->use();
        glm::vec4 background = glm::vec4(pow(backgroundColor[0], gamma),pow(backgroundColor[1], gamma),pow(backgroundColor[2], gamma),pow(backgroundColor[3], gamma));
        backgroundShader->setVec4("color", background);
        RenderQuad();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);

        for (auto & m : scene->getMeshes()) {

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, m->getPosition());
            model = glm::rotate(model, glm::radians(m->getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(m->getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(m->getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, m->getScale());

            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);

            m->getShader()->use();
            m->getShader()->setVec3("viewPos", camera->getPosition());
            m->getShader()->setInt("width", int(width));
            m->getShader()->setInt("height", int(height));

            m->getShader()->setInt(std::string("nbAmbiantLights"), scene->getAmbiantLights().size());
            for (size_t i = 0; i < scene->getAmbiantLights().size(); ++i) {
                m->getShader()->setVec3(std::string("ambiantLights[").append(std::to_string(i)).append("].Color"), scene->getAmbiantLights().at(i)->getColor());
            }

            m->getShader()->setInt(std::string("nbDirectionalLights"), scene->getDirectionalLights().size());
            for (size_t i = 0; i < scene->getDirectionalLights().size(); ++i) {
                m->getShader()->setVec3(std::string("directionalLights[").append(std::to_string(i)).append("].Color"), scene->getDirectionalLights().at(i)->getColor());
                m->getShader()->setVec3(std::string("directionalLights[").append(std::to_string(i)).append("].Direction"), scene->getDirectionalLights().at(i)->getForwardVector());
            }

            m->getShader()->setInt(std::string("nbPointLights"), scene->getPointLights().size());
            for (size_t i = 0; i < scene->getPointLights().size(); ++i) {
                m->getShader()->setVec3(std::string("pointLights[").append(std::to_string(i)).append("].Color"), scene->getPointLights().at(i)->getColor());
                m->getShader()->setVec3(std::string("pointLights[").append(std::to_string(i)).append("].Position"), scene->getPointLights().at(i)->getPosition());
                m->getShader()->setFloat(std::string("pointLights[").append(std::to_string(i)).append("].Constant"), scene->getPointLights().at(i)->getConstant());
                m->getShader()->setFloat(std::string("pointLights[").append(std::to_string(i)).append("].Linear"), scene->getPointLights().at(i)->getLinear());
                m->getShader()->setFloat(std::string("pointLights[").append(std::to_string(i)).append("].Quadratic"), scene->getPointLights().at(i)->getQuadratic());
            }

            m->getShader()->use();
            m->getShader()->setMat4fv("model", model);
            m->getShader()->setMat4fv("view", view);
            m->getShader()->setMat4fv("projection", projection);

            glBindVertexArray(m->getVAO());
            glDrawElements(GL_TRIANGLES, m->getIndices().size(), GL_UNSIGNED_INT, 0);

            if (renderOutline) {
                glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
                glStencilMask(0x00);
                glDisable(GL_DEPTH_TEST);

                scaleShader->use();
                scaleShader->setMat4fv("model", model);
                scaleShader->setMat4fv("view", view);
                scaleShader->setMat4fv("projection", projection);
                glDrawElements(GL_TRIANGLES, m->getIndices().size(), GL_UNSIGNED_INT, 0);

                glStencilMask(0xFF);
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glEnable(GL_DEPTH_TEST);
            }
        }


        bool horizontal = true, first_iteration = true;
        unsigned int amount = 20;
        blurShader->use();
        for (unsigned int i = 0; i < amount; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
            glViewport(0,0,width/4,height/4);
            blurShader->setInt("horizontal", horizontal);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(
                    GL_TEXTURE_2D, first_iteration ? textureBloomBuffer : pingpongBuffer[!horizontal]
            );
            blurShader->setInt("image", 0);
            RenderQuad();
            horizontal = !horizontal;
            if (first_iteration)
                first_iteration = false;
        }

        glViewport(0,0,width,height);
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader->use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
        screenShader->setInt("colorTexture", 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);
        screenShader->setInt("bloomTexture", 1);

        screenShader->setFloat("gamma", gamma);
        screenShader->setFloat("exposition", exposition);
        screenShader->setFloat("bloom", bloom);
        screenShader->setInt("width", width);
        screenShader->setInt("height", height);
        RenderQuad();
    }

    void Renderer::setScreenSize(int _width, int _height)
    {
        if (width == _width && height == _height)
            return;

        width = _width;
        height = _height;
        generateBuffers();
    }

    void Renderer::generateScreenVAO()
    {
        std::vector<float> screenVertices = {
                -1.f, -1.f, 0.f, 0.f, 0.f,
                1.f, -1.f, 0.f, 1.f, 0.f,
                -1.f, 1.f, 0.f, 0.f, 1.f,
                1.f, 1.f, 0.f, 1.f, 1.f,
        };
        std::vector<unsigned int> screenIndices = { 0, 1, 2, 2, 1, 3 };

        glGenBuffers(1, &screenVBO);
        glGenVertexArrays(1, &screenVAO);
        glGenBuffers(1, &screenEBO);

        // ..:: Initialization code :: ..
        // 1. bind Vertex Array Object
        glBindVertexArray(screenVAO);
        // 2. copy our vertices array in a vertex buffer for OpenGL to use
        glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
        glBufferData(GL_ARRAY_BUFFER, screenVertices.size()*sizeof(float), screenVertices.data(), GL_STATIC_DRAW);
        // 3. copy our index array in a element buffer for OpenGL to use
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, screenIndices.size()*sizeof(unsigned int), screenIndices.data(), GL_STATIC_DRAW);
        // 4. then set the vertex attributes pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void Renderer::generateBuffers()
    {
        glDeleteFramebuffers(1, &framebuffer);
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glDeleteTextures(1, &textureColorBuffer);
        textureColorBuffer = generateTexture();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

        glDeleteTextures(1, &textureBloomBuffer);
        textureBloomBuffer = generateTexture();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, textureBloomBuffer, 0);

        unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        glDrawBuffers(2, attachments);

        glDeleteRenderbuffers(1, &rbo);
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glDeleteFramebuffers(2, pingpongFBO);
        glDeleteTextures(2, pingpongBuffer);
        glGenFramebuffers(2, pingpongFBO);
        glGenTextures(2, pingpongBuffer);
        for (unsigned int i = 0; i < 2; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
            glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
            glTexImage2D(
                    GL_TEXTURE_2D, 0, GL_RGBA16F, width/4, height/4, 0, GL_RGBA, GL_FLOAT, NULL
            );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(
                    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0
            );
        }
    }

    unsigned int Renderer::generateTexture() const
    {
        unsigned int texture;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach it to currently bound framebuffer object

        glBindTexture(GL_TEXTURE_2D, 0);

        return texture;
    }

    void Renderer::RenderQuad() const
    {
        glBindVertexArray(screenVAO);
        glDisable(GL_DEPTH_TEST);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

}