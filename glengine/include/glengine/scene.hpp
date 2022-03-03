#ifndef SCENE_HPP
#define SCENE_HPP

#include <iostream>
#include "mesh.hpp"
#include "camera.hpp"

namespace GLEngine {

    class Scene {

    private:
        std::vector<Mesh*> meshes;
        Camera* camera;

    public:
        Scene();

        void update(double frameTime);

        inline void addMesh(Mesh* m) {meshes.push_back(m);}

        inline Camera* getCamera() const {return camera;}
        inline std::vector<Mesh*> getMeshes() const {return meshes;}

    };

}

#endif