#include "scene/animation/quatAnimation.hpp"

#include <glm/ext/quaternion_float.hpp>

QuatAnimation::QuatAnimation(Accessor<glm::quat> *dataAccessor, Accessor<float> *timeAccessor, Node &target, InterpolationType type) :
dataAccessor(dataAccessor),
Animation(target, type, timeAccessor)
{
    if(type != LINEAR)
        LOGE("Type %s not supported, using LINEAR\n", type == STEP ? "STEP" : "CUBICSPLINE");
    this->type = type;
    currentKeyframeIndex = 0;
}

void QuatAnimation::update(float deltaT)
{
    currentTime += deltaT;

    //LOGD("Current time %f, frame time %f\n",currentTime, (*timeAccessor)[currentKeyframeIndex]);

    while(currentTime >= (*timeAccessor)[currentKeyframeIndex])
    {
        currentKeyframeIndex++;
        if(currentKeyframeIndex >= timeAccessor->getCount())
        {
            currentTime = 0.0f;
            currentKeyframeIndex = 0;
        }
    }
    
    if(currentKeyframeIndex == 0)
    {
        LOGD("Returning...\n");
        return;
    }

    float previousTime = (*timeAccessor)[currentKeyframeIndex - 1];
    float nextTime = (*timeAccessor)[currentKeyframeIndex];

    float interpolationValue = (currentTime - previousTime) / (nextTime - previousTime);
    if(interpolationValue < 0.0f || interpolationValue > 1.0f)
        LOGE("Interpolation value was %f\n", interpolationValue);

    Transform t = target.getTransform();
    t.rotation = glm::slerp((*dataAccessor)[currentKeyframeIndex - 1], (*dataAccessor)[currentKeyframeIndex], interpolationValue);
    target.setTransform(t);
}