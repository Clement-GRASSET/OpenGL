#include <glm/glm.hpp>

namespace GLEngine {

    class Camera {

    private:
        glm::vec3 position;
        glm::vec3 rotation;

    public:
        Camera();
        ~Camera() = default;

        inline glm::vec3 getPosition() const {return position;}
        inline glm::vec3 getRotation() const {return rotation;}
        inline void setPosition(glm::vec3 _position) {position = _position;}
        inline void setRotation(glm::vec3 _rotation) {rotation = _rotation;}

        glm::vec3 getForwardVector() const;
        glm::vec3 getUpVector() const;
        glm::vec3 getRightVector() const;

    };

}