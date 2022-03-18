#include <glengine/scene.hpp>

class CustomScene : public GLEngine::Scene {

public:
    CustomScene();
    ~CustomScene();

    void update(double frameTime) override;

};