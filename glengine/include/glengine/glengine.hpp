#ifndef __GLENGINE_HPP__
#define __GLENGINE_HPP__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GL/gl.h>
#include <iostream>
#include <stdexcept>
#include <glengine/config.hpp>

/**
 * \namespace GLEngine
 * @brief Espace de nom pour GLEngine
 *
 * blabla
 */
namespace GLEngine {
	static void GLClearError() {
		while (glGetError() != GL_NO_ERROR);
	}

	static bool GLLogCall(const char* function, const char* file, int line) {
		while (GLenum error = glGetError()) {
			std::cout << "[OpenGL Error] (" << error << ") " << function << std::endl
				<< file <<" L" << line << std::endl;
			return false;
		}
		return true;
	}
} // namespace GLEngine

#define ASSERT(x) if ((!x)) throw std::runtime_error("a gl error occured");

#ifdef DEBUG
#define GLCALL(x) GLEngine::GLClearError();\
	x;\
	ASSERT(GLEngine::GLLogCall(#x, __FILE__, __LINE__));
#else
#define GLCALL(x) x;
#endif



#endif // !
