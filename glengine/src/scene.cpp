#include "glengine/scene.hpp"

namespace GLEngine {

    Scene::Scene() {
        camera = new Camera();
    }

    void Scene::update(double frameTime) {
        camera->setRotation(camera->getRotation() + glm::vec3(0.f, frameTime*100, 0.f));
    }

}