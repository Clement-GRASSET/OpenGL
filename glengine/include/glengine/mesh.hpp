#ifndef mesh
#define mesh

#include <vector>
#include "shader.hpp"

namespace GLEngine {

    class Mesh {

    private:
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        Shader * shader;
        unsigned int VAO, VBO, EBO;
        glm::vec3 position, scale;

    public:
        Mesh();
        ~Mesh();
        void render(glm::mat4 &view, glm::mat4 &projection) const;
        Shader *getShader() const;
        void setShader(Shader *shader);

        inline void setScale(glm::vec3 _scale) { scale = _scale; }
        inline void setPosition(glm::vec3 _position) { position = _position; }

        inline void setVertices(std::vector<float> _vertices) { vertices = _vertices; }
        inline void setIndices(std::vector<unsigned int> _indices) { indices = _indices; }

        void debug() const;

    };

}

#endif