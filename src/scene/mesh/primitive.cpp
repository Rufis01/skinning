#include "scene/mesh/primitive.hpp"

#include "log.h"

Primitive::Primitive(std::vector<Vertex> verticies, std::vector<short> indicies, Texture *texture) : verticies(verticies), indicies(indicies), texture(texture)
{
}

void Primitive::draw()
{
}

GLuint Primitive::getVertexBufferSize()
{
    return verticies.size() * sizeof(Vertex);
}

GLuint Primitive::getIndexBufferSize()
{
    return indicies.size() * sizeof(short);
}

void Primitive::bindTexture()
{
    if(texture)
        texture->bind();
}

Vertex *Primitive::getVertexData()
{
    return verticies.data();
}

short *Primitive::getIndexData()
{
    return indicies.data();
}

int Primitive::getNumberOfIndicies()
{
    return indicies.size();
}
