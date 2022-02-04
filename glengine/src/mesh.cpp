#include "glengine/mesh.hpp"
#include "glengine/config.hpp"

namespace GLEngine {

    Mesh::Mesh() {
        position = glm::vec3(0.f, 0.f, 0.f);
        scale = glm::vec3(1.f, 1.f, 1.f);

        vertices = {};
        indices = {};

        shader = new Shader(
                std::string(_resources_directory).append("shaders/default.vert").c_str(),
                std::string(_resources_directory).append("shaders/default.frag").c_str()
        );

        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &EBO);
    }

    Mesh::~Mesh() {
        delete shader;
    }

    void Mesh::render(glm::mat4 &view, glm::mat4 &projection) const {
        // ..:: Initialization code :: ..
        // 1. bind Vertex Array Object
        glBindVertexArray(VAO);
        // 2. copy our vertices array in a vertex buffer for OpenGL to use
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
        // 3. copy our index array in a element buffer for OpenGL to use
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);
        // 4. then set the vertex attributes pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, scale);
        model = glm::translate(model, position);
        //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        shader->setMat4fv("model", model);
        shader->setMat4fv("view", view);
        shader->setMat4fv("projection", projection);
        shader->use();

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }

    Shader *Mesh::getShader() const {
        return shader;
    }

    void Mesh::setShader(Shader *shader) {
        Mesh::shader = shader;
    }


}