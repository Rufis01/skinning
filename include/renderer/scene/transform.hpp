#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/ext/quaternion_float.hpp>

struct Transform
{
    glm::vec3 translation;
    glm::vec3 scale;
    glm::quat rotation;
};

#endif