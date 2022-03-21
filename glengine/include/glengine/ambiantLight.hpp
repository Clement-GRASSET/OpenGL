#ifndef AMBIANT_LIGHT_HPP
#define AMBIANT_LIGHT_HPP

#include <glengine/light.hpp>

namespace GLEngine {

    class AmbiantLight : public Light {

    private:

    public:
        AmbiantLight()
        : Light()
        { setColor(glm::vec3(0.05)); }

    };

}

#endif