#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glengine/object.hpp>

namespace GLEngine {

    class Light : public Object {

    private:
        glm::vec3 color;

    public:
        Light()
        : color(glm::vec3(1,1,1))
        {}

        inline glm::vec3 getColor() { return color; }
        inline void setColor(glm::vec3 _color) { color = _color; }

    };

}

#endif