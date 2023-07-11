#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include <vector>

#include <GL/glew.h>

#include "material.hpp"
#include "vertex.hpp"

class Primitive
{
    public:
    Primitive(std::vector<Vertex> verticies, std::vector<short> indicies);
    void draw();
    GLuint getVertexBufferSize();
    GLuint getIndexBufferSize();

    Vertex *getVertexData();
    short *getIndexData();
    int getNumberOfIndicies();

    private:
    std::vector<Vertex> verticies;
    std::vector<short> indicies;

    //Material &material;

    int mode;
};

#endif