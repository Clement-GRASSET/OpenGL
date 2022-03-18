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

}