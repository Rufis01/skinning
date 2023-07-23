#ifndef VEC3ANIMATION_HPP
#define VEC3ANIMATION_HPP

#include <glm/glm.hpp>

#include "animation.hpp"
#include "accessor/accessor.hpp"

class Vec3Animation : public Animation
{
    public:
    typedef enum
    {
        SCALE,
        TRANSLATION
    } Path;

    Vec3Animation(Accessor<glm::vec3> *dataAccessor, Accessor<float> *timeAccessor, Node &target, Path path, InterpolationType type = LINEAR);
    virtual void update(float deltaT);
    ~Vec3Animation() = default;

    private:
    Accessor<glm::vec3> *dataAccessor;
    Path path;
};

#endif