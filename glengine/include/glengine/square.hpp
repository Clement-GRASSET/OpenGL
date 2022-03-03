#ifndef SQUARE_HPP
#define SQUARE_HPP

#include "mesh.hpp"

namespace GLEngine {

    class Square : public Mesh {

    private:

    public:
        Square()
        : Mesh()
        {
            setVertices(std::vector<float>({
                                0.5f,  0.5f, 0.0f,  // top right
                                0.5f, -0.5f, 0.0f,  // bottom right
                                -0.5f, -0.5f, 0.0f,  // bottom left
                                -0.5f,  0.5f, 0.0f   // top left
                        }));

            setIndices(std::vector<unsigned int>({
                                                         0, 1, 3,   // first triangle
                                                         1, 2, 3    // second triangle
                                                 }));
        }

    };

}

#endif