#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <glengine/shader.hpp>
#include <glengine/object.hpp>

namespace GLEngine {

    class Mesh : public Object {

    private:
        std::vector<float> normals;
        Shader * shader;

    protected:
        unsigned int VAO, VBO, EBO;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        void computeNormals();
        virtual void generateBuffers();

    public:
        Mesh();
        ~Mesh();
        Shader *getShader() const;
        void setShader(Shader *shader);

        inline void setVertices(std::vector<float> _vertices) { vertices = _vertices; }
        inline void setIndices(std::vector<unsigned int> _indices) { indices = _indices; }

        inline unsigned int getVAO() const { return VAO; }
        inline std::vector<float> getVertices() const { return vertices; }
        inline std::vector<unsigned int> getIndices() const { return indices; }

        static Mesh* load(const char* path);

        void debug() const;

    };

}

#endif