#include <glengine/mesh.hpp>
#include <glengine/shaderManager.hpp>

extern const char* _resources_directory;

namespace GLEngine {

    Mesh::Mesh()
    {
        vertices = {};
        indices = {};

        shader = ShaderManager::getDefault();

        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &EBO);
    }

    Mesh::~Mesh() {

    }

    Shader *Mesh::getShader() const {
        return shader;
    }

    void Mesh::setShader(Shader *shader) {
        Mesh::shader = shader;
    }

    void Mesh::debug() const {
        std::cout << "Vertices :" << std::endl;
        for (size_t i = 0; i < vertices.size(); ++i) {
            std::cout << vertices[i] << ", ";
            if (i%3 == 2)
                std::cout << std::endl;
        }

        std::cout << "Indices :" << std::endl;
        for (size_t i = 0; i < indices.size(); ++i) {
            std::cout << indices[i] << ", ";
            if (i%3 == 2)
                std::cout << std::endl;
        }
    }

    Mesh* Mesh::load(const char *path) {
        Mesh * m = new Mesh();
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        std::ifstream meshFile;
        try {
            meshFile.open(path, std::ios::binary);
            char c;

            while(!meshFile.eof()) {
                meshFile >> c;

                if (c == 'v') {
                    float x, y, z;
                    meshFile >> x >> y >> z;
                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z);
                    //std::cout << x << ", " << y << ", " << z << std::endl;
                } else if (c == 'f') {
                    unsigned int f1, f2, f3;
                    meshFile >> f1 >> f2 >> f3;
                    indices.push_back(f1 - 1);
                    indices.push_back(f2 - 1);
                    indices.push_back(f3 - 1);
                    //std::cout << f1 << ", " << f2 << ", " << f3 << std::endl;
                } else {
                    while (meshFile.get() != '\n') {}
                }
            }

        } catch (std::ifstream::failure& e) {
            std::cerr << "Impossible de charger le mesh " << path << std::endl;
            return m;
        }

        m->setVertices(vertices);
        m->setIndices(indices);
        m->computeNormals();
        m->generateBuffers();
        return m;
    }

    void Mesh::computeNormals() {
        for (size_t i = 0; i < vertices.size(); ++i) {
            normals.push_back(0);
        }

        for (size_t i = 0; i < indices.size(); i = i+3) {
            unsigned int i1 = indices[i], i2 = indices[i+1], i3 = indices[i+2];
            //std::cout << i1 << ", " << i2 << ", " << i3 << std::endl;
            glm::vec3 v1, v2, v3;
            v1 = glm::vec3(vertices[i1*3], vertices[i1*3+1], vertices[i1*3+2]);
            v2 = glm::vec3(vertices[i2*3], vertices[i2*3+1], vertices[i2*3+2]);
            v3 = glm::vec3(vertices[i3*3], vertices[i3*3+1], vertices[i3*3+2]);
            glm::vec3 normal = glm::cross(v2-v1, v3-v1);
            normals[i1*3] += normal[0];
            normals[i1*3+1] += normal[1];
            normals[i1*3+2] += normal[2];
            normals[i2*3] += normal[0];
            normals[i2*3+1] += normal[1];
            normals[i2*3+2] += normal[2];
            normals[i3*3] += normal[0];
            normals[i3*3+1] += normal[1];
            normals[i3*3+2] += normal[2];
        }

        for (size_t i = 0; i < normals.size(); i = i+3) {
            glm::vec3 normal = glm::vec3(normals[i], normals[i+1], normals[i+2]);
            normal = glm::normalize(normal);
            normals[i] = normal[0];
            normals[i+1] = normal[1];
            normals[i+2] = normal[2];
        }

        /*for (int i = 0; i < normals.size(); i+=3) {
            std::cout << normals[i] << ", " << normals[i+1] << ", " << normals[i+2] << std::endl;
        }*/
    }

    void Mesh::generateBuffers() {
        std::vector<float> data;
        data.reserve(vertices.size() + normals.size());
        data.insert(data.end(), vertices.begin(), vertices.end());
        data.insert(data.end(), normals.begin(), normals.end());

        // ..:: Initialization code :: ..
        // 1. bind Vertex Array Object
        glBindVertexArray(VAO);
        // 2. copy our vertices array in a vertex buffer for OpenGL to use
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), data.data(), GL_STATIC_DRAW);
        // 3. copy our index array in a element buffer for OpenGL to use
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        // 4. then set the vertex attributes pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(vertices.size()*sizeof(float)));
        glEnableVertexAttribArray(1);
    }


}