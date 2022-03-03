#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "mesh.hpp"

namespace GLEngine {

    class Terrain : public Mesh {

    public:
        Terrain()
        : Mesh()
        {
            float radius = 2.f;
            unsigned int nbSegments = 16;

            std::vector<float> vertices;
            std::vector<unsigned int> indices;

            vertices.push_back(0.f);
            vertices.push_back(0.f);
            vertices.push_back(0.f);

            for (unsigned int i = 0; i < nbSegments; ++i) {
                float angle = glm::radians(float(i)/float(nbSegments) * 360.f);

                vertices.push_back(std::cos(angle) * radius);
                vertices.push_back(0.f);
                vertices.push_back(std::sin(angle) * radius);

                indices.push_back(0);
                indices.push_back(i+1);
                indices.push_back((i+1 == nbSegments) ? 1 : i+2);
            }

            setVertices(vertices);
            setIndices(indices);
            debug();
        }

    };

}

#endif