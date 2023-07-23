#include "scene/mesh/texture.hpp"

Texture::Texture(int width, int height, std::vector<unsigned char> &data) :
width(width),
height(height),
data(data)
{
}

void Texture::uploadToGPU()
{
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureHandle); 
    glBindTexture(GL_TEXTURE_2D, textureHandle);  
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);  
}

void Texture::bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureHandle); 
}
