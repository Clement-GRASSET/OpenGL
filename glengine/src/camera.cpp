#include <glengine/camera.hpp>

namespace GLEngine {

    Camera::Camera()
    : position(glm::vec3(0,0,0)), rotation(glm::vec3(0,0,0))
    {}

    glm::vec3 Camera::getForwardVector() const
    {
        return {
                -cos(-M_PI*rotation.z/180) * sin(-M_PI*rotation.y/180),
                sin(-M_PI*rotation.z/180),
                -cos(-M_PI*rotation.z/180) * cos(-M_PI*rotation.y/180)
        };
    }

    glm::vec3 Camera::getUpVector() const
    {
        return {
                sin(-M_PI*rotation.z/180) * sin(-M_PI*rotation.y/180),
                cos(-M_PI*rotation.z/180),
                sin(-M_PI*rotation.z/180) * cos(-M_PI*rotation.y/180)
        };
    }

    glm::vec3 Camera::getRightVector() const
    {
        return {
                cos(-M_PI*rotation.y/180),
                0,
                -sin(-M_PI*rotation.y/180)
        };
    }

}