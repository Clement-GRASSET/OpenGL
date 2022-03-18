#ifndef DISC_HPP
#define DISC_HPP

#include <cmath>

#include "mesh.hpp"

namespace GLEngine {

    class Disk : public Mesh {

    public:
        Disk(float radius = 0.5f, unsigned int nbSegments = 16)
        : Mesh()
        {
            std::vector<float> vertices;
            std::vector<unsigned int> indices;

            vertices.push_back(0.f);
            vertices.push_back(0.f);
            vertices.push_back(0.f);

            for (unsigned int i = 0; i < nbSegments; ++i) {
                float angle = glm::radians(float(i)/float(nbSegments) * 360.f);

                vertices.push_back(std::cos(angle) * radius);
                vertices.push_back(std::sin(angle) * radius);
                vertices.push_back(0.f);

                indices.push_back(0);
                indices.push_back(i+1);
                indices.push_back((i+1 == nbSegments) ? 1 : i+2);
            }

            setVertices(vertices);
            setIndices(indices);
            computeNormals();
            generateBuffers();
            //debug();
        }

    };

}

#endif