#ifndef SCENE_HPP
#define SCENE_HPP

#include <iostream>
#include <glengine/mesh.hpp>
#include <glengine/ambiantLight.hpp>
#include <glengine/directionalLight.hpp>
#include <glengine/pointLight.hpp>
#include <glengine/camera.hpp>

namespace GLEngine {

    class Scene {

    private:
        std::vector<Mesh*> meshes;
        std::vector<AmbiantLight*> ambiantLights;
        std::vector<DirectionalLight*> directionalLights;
        std::vector<PointLight*> pointLights;
        Camera* camera;

    public:
        Scene();

        virtual void update(double frameTime);

        inline std::vector<Mesh*> getMeshes() const {return meshes;}
        inline std::vector<AmbiantLight*> getAmbiantLights() const {return ambiantLights;}
        inline std::vector<DirectionalLight*> getDirectionalLights() const {return directionalLights;}
        inline std::vector<PointLight*> getPointLights() const {return pointLights;}

        inline void addMesh(Mesh* m) {meshes.push_back(m);}
        inline void addAmbiantLight(AmbiantLight* l) {ambiantLights.push_back(l);}
        inline void addDirectionalLight(DirectionalLight* l) {directionalLights.push_back(l);}
        inline void addPointLight(PointLight* l) {pointLights.push_back(l);}

        void removeMesh(Mesh* m);
        void removeAmbiantLight(AmbiantLight* l);
        void removeDirectionalLight(DirectionalLight* l);
        void removePointLight(PointLight* l);

        inline Camera* getCamera() const {return camera;}
    };

}

#endif