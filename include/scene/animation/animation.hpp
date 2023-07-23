#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "accessor/accessor.hpp"
#include "scene/node.hpp"

class Animation
{
    public:
    typedef enum
    {
        STEP,
        LINEAR,
        CUBICSPLINE
    }InterpolationType;

    Animation(Node &target, InterpolationType type, Accessor<float> *timeAccessor);
    virtual void update(float deltaT) = 0;
    virtual ~Animation() = 0;

    protected:
    Node &target;
    InterpolationType type;
    Accessor<float> *timeAccessor;

    float currentTime = 0.0f;
    unsigned int currentKeyframeIndex = 0;
};

#endif