#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glm/glm.hpp>

namespace GLEngine {

    class Object {

    private:
        glm::vec3 position, scale, rotation;

    public:
        Object();
        virtual ~Object();

        inline glm::vec3 getPosition() const { return position; }
        inline glm::vec3 getScale() const { return scale; }
        inline glm::vec3 getRotation() const { return rotation; }
        inline void setPosition(glm::vec3 _position) { position = _position; }
        inline void setScale(glm::vec3 _scale) { scale = _scale; }
        inline void setRotation(glm::vec3 _rotation) { rotation = _rotation; }

        glm::vec3 getForwardVector() const;
        glm::vec3 getUpVector() const;
        glm::vec3 getRightVector() const;

    };

}

#endif