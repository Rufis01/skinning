#include "scene/mesh.hpp"

Mesh::Mesh(std::vector<Primitive> primitives) : primitives(primitives)
{
    int sz = primitives.size();
    vertexBufferHandles.resize(sz);
    indexBufferHandles.resize(sz);
}

std::vector<Primitive> &Mesh::getPrimitives()
{
    return primitives;
}

void Mesh::uploadToGPU()
{
    unsigned int prims = primitives.size();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

    glGenBuffers(prims, vertexBufferHandles.data());
    glGenBuffers(prims, indexBufferHandles.data());

    for(int i = 0; i < prims; i++)
    {
        Primitive *p = &primitives[i];

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferHandles[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p->getIndexBufferSize(), p->getIndexData(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[i]); 
        glBufferData(GL_ARRAY_BUFFER, p->getVertexBufferSize(), p->getVertexData(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::draw()
{
	glBindVertexArray(VAO);

    for(int i = 0; i < primitives.size(); i++)
    {
        Primitive *p = &primitives[i];

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferHandles[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[i]); 

        glDrawElements(GL_TRIANGLES, p->getNumberOfIndicies(), GL_UNSIGNED_SHORT, 0);
    }
    
    glBindVertexArray(0);
}