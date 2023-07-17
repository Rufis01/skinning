#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec4 joints;
    glm::vec4 weights;
};

#endif