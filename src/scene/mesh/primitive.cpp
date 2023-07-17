#include "scene/mesh/primitive.hpp"

#include "log.h"

Primitive::Primitive(std::vector<Vertex> verticies, std::vector<short> indicies) : verticies(verticies), indicies(indicies)
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
