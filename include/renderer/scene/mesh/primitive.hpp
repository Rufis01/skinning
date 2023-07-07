#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include <vector>

#include "material.hpp"
#include "vertex.hpp"

class Primitive
{
    public:
    Primitive(std::vector<Vertex> verticies, std::vector<short> indicies);

    private:
    std::vector<Vertex> verticies;
    std::vector<short> indicies;

    //Material &material;

    int mode;
};

#endif