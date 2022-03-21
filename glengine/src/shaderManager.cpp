#include <glengine/shaderManager.hpp>

extern const char* _resources_directory;

namespace GLEngine {

    Shader* ShaderManager::defaultShader = nullptr;

    ShaderManager::ShaderManager()
    {
        addShader("Default", getDefault());
    }

    Shader *ShaderManager::getDefault()
    {
        if (defaultShader != nullptr)
            return defaultShader;

        defaultShader = new Shader(
                std::string(_resources_directory).append("shaders/default.vert").c_str(),
                std::string(_resources_directory).append("shaders/default.frag").c_str()
        );

        return defaultShader;
    }

    void ShaderManager::addShader(std::string name, Shader *shader)
    {
        shaders.insert(std::pair<std::string, Shader*>(name, shader));
    }

}