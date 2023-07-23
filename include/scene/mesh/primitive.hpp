#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include <vector>

#include <GL/glew.h>

#include "material.hpp"
#include "vertex.hpp"
#include "texture.hpp"

class Primitive
{
    public:
    Primitive(std::vector<Vertex> verticies, std::vector<short> indicies, Texture *texture);
    void draw();
    GLuint getVertexBufferSize();
    GLuint getIndexBufferSize();
    void bindTexture();

    Vertex *getVertexData();
    short *getIndexData();
    int getNumberOfIndicies();

    private:
    std::vector<Vertex> verticies;
    std::vector<short> indicies;

    Texture *texture;
};

#endif