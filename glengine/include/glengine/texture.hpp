#ifndef TEXTURE_HPP
#define TEXTURE_HPP

namespace GLEngine {

    class Texture {

    public:
        ~Texture();

        static Texture* load(const char *path);

        unsigned int getTexture() const {return texture;};

    private:
        Texture();

        unsigned int texture;

    };

}

#endif