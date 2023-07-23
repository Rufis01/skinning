#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <vector>
#include <GL/glew.h>

class Texture
{
    public:
    Texture(int width, int height, std::vector<unsigned char> &data);
    void uploadToGPU();
    void bind();

    private:
    int width;
    int height;

    std::vector<unsigned char> data;

    GLuint textureHandle;
};

#endif