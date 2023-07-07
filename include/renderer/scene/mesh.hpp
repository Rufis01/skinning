#ifndef MESH_HPP
#define MESH_HPP

#include "v_attachable.hpp"

#include <vector>

#include "mesh/primitive.hpp"

class Mesh : public Attachable
{
    public:
    Mesh(std::vector<Primitive> primitives);
    private:
    std::vector<Primitive> primitives;
};

#endif
