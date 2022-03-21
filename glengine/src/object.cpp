#include <glengine/object.hpp>

namespace GLEngine {

    Object::Object()
    : position(glm::vec3(0.f, 0.f, 0.f)), scale(glm::vec3(1.f, 1.f, 1.f)), rotation(glm::vec3(0.f, 0.f, 0.f))
    {}

    glm::vec3 Object::getForwardVector() const
    {
        return {
                -cos(-M_PI*rotation.z/180) * sin(-M_PI*rotation.y/180),
                sin(-M_PI*rotation.z/180),
                -cos(-M_PI*rotation.z/180) * cos(-M_PI*rotation.y/180)
        };
    }

    glm::vec3 Object::getUpVector() const
    {
        return {
                sin(-M_PI*rotation.z/180) * sin(-M_PI*rotation.y/180),
                cos(-M_PI*rotation.z/180),
                sin(-M_PI*rotation.z/180) * cos(-M_PI*rotation.y/180)
        };
    }

    glm::vec3 Object::getRightVector() const
    {
        return {
                cos(-M_PI*rotation.y/180),
                0,
                -sin(-M_PI*rotation.y/180)
        };
    }

    Object::~Object()
    = default;

}