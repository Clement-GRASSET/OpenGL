#ifndef SHADER_MANAGER_HPP
#define SHADER_MANAGER_HPP

#include <iostream>
#include <map>
#include <glengine/shader.hpp>

namespace GLEngine {

    class ShaderManager {

    private:
        static Shader* defaultShader;

        std::map<std::string, Shader*> shaders;

    public:
        ShaderManager();

        void addShader(std::string name, Shader* shader);
        inline std::map<std::string, Shader*> getShaders() {return shaders;}

        static Shader* getDefault();
    };

}

#endif
