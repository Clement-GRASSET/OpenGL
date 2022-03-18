#include <tp01/customScene.hpp>

CustomScene::CustomScene()
: Scene()
{
    //camera = new Camera();

    //getCamera()->setPosition(glm::vec3(0, 0, -3));
    //getCamera()->setRotation(glm::vec3(0,0,0));
}

CustomScene::~CustomScene()
{

}

void CustomScene::update(double frameTime) {
    Scene::update(frameTime);
}
