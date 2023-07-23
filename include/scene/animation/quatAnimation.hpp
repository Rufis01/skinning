#ifndef QUATANIMATION_HPP
#define QUATANIMATION_HPP

#include <glm/glm.hpp>
#include <glm/ext/quaternion_common.hpp>

#include "animation.hpp"
#include "accessor/accessor.hpp"


class QuatAnimation : public Animation
{
    public:
    QuatAnimation(Accessor<glm::quat> *dataAccessor, Accessor<float> *timeAccessor, Node &target, InterpolationType type = LINEAR);
    virtual void update(float deltaT);
    ~QuatAnimation() = default;

    private:
    Accessor<glm::quat> *dataAccessor;
};

#endif