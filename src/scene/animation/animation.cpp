#include "scene/animation/animation.hpp"

Animation::Animation(Node &target, InterpolationType type, Accessor<float> *timeAccessor) :
target(target),
type(type),
timeAccessor(timeAccessor)
{
}

Animation::~Animation()
{
}