#include <glengine/texture.hpp>

#include <iostream>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"


namespace GLEngine {

    Texture::Texture()
    : texture(0)
    {

    }

    Texture::~Texture()
    {

    }

    Texture* Texture::load(const char *path)
    {
        Texture* texture = new Texture();

        //glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &texture->texture);
        glBindTexture(GL_TEXTURE_2D, texture->texture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int width, height, nrChannels;
        unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
        if (data)
        {
            switch (nrChannels) {
                case 3: {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    break;
                }
                case 4: {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                    break;
                }
                default: {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    break;
                }
            }

            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture : " << path << std::endl;
        }
        stbi_image_free(data);

        return texture;
    }

}