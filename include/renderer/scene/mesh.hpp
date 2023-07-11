#ifndef MESH_HPP
#define MESH_HPP

#include "v_attachable.hpp"

#include <vector>

#include "mesh/primitive.hpp"

class Mesh : public Attachable
{
    public:
    Mesh(std::vector<Primitive> primitives);
    std::vector<Primitive> &getPrimitives();

    void uploadToGPU();
    void draw();

    private:
    std::vector<Primitive> primitives;

    GLuint VAO;
    std::vector<GLuint> vertexBufferHandles, indexBufferHandles;
};

#endif
