#include "scene/mesh/primitive.hpp"

#include "log.h"

Primitive::Primitive(std::vector<Vertex> verticies, std::vector<short> indicies) : verticies(verticies), indicies(indicies)
{
}
/*
void Primitive::uploadToGPU()
{
	glGenVertexArrays(1, &varr);
	glBindVertexArray(varr);
    glGenBuffers(1, &vbuff);

    glBindBuffer(GL_ARRAY_BUFFER, vbuff); 
    glBufferData(GL_ARRAY_BUFFER, this->verticies.size() * sizeof(Vertex), this->verticies.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    glGenBuffers(1, &ibuff);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuff);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indicies.size() * sizeof(short), this->indicies.data(), GL_STATIC_DRAW);

    //glBindVertexArray(0);   //Unbinds the VAO
    //glBindBuffer(GL_ARRAY_BUFFER, 0);   //Unbinds the VBO
}*/

void Primitive::draw()
{
    //glBindVertexArray(this->varr);
    //glDrawElements(GL_TRIANGLES, ibuff, GL_UNSIGNED_SHORT, NULL);
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
