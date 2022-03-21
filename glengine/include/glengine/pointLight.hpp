#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include <glengine/light.hpp>

namespace GLEngine {

    class PointLight : public Light {

    private:
        float constant, linear, quadratic;

    public:
        PointLight()
        : constant(0), linear(1), quadratic(0)
        {}

        inline float getConstant() {return constant;}
        inline float getLinear() {return linear;}
        inline float getQuadratic() {return quadratic;}
        inline void setConstant(float _constant) {constant = _constant;}
        inline void setLinear(float _linear) {linear = _linear;}
        inline void setQuadratic(float _quadratic) {quadratic = _quadratic;}

    };

}

#endif
