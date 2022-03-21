#include "glengine/scene.hpp"

namespace GLEngine {

    Scene::Scene()
    : camera(nullptr)
    {
        camera = new Camera();
        camera->setPosition(glm::vec3(0, 0, 3));
    }

    void Scene::update(double frameTime) {

    }

    void Scene::removeMesh(Mesh *m)
    {
        for (auto it = meshes.begin(); it != meshes.end() ; ++it) {
            Mesh* current = *it;
            if (current == m) {
                meshes.erase(it);
                delete m;
                return;
            }
        }
    }

    void Scene::removeAmbiantLight(AmbiantLight *l)
    {
        for (auto it = ambiantLights.begin(); it != ambiantLights.end() ; ++it) {
            AmbiantLight* current = *it;
            if (current == l) {
                ambiantLights.erase(it);
                delete l;
                return;
            }
        }
    }

    void Scene::removeDirectionalLight(DirectionalLight *l)
    {
        for (auto it = directionalLights.begin(); it != directionalLights.end() ; ++it) {
            DirectionalLight* current = *it;
            if (current == l) {
                directionalLights.erase(it);
                delete l;
                return;
            }
        }
    }

}