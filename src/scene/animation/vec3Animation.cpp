#include "scene/animation/vec3Animation.hpp"

Vec3Animation::Vec3Animation(Accessor<glm::vec3> *dataAccessor, Accessor<float> *timeAccessor, Node &target, Path path, InterpolationType type) :
Animation(target, type, timeAccessor),
dataAccessor(dataAccessor),
path(path)
{
    this->timeAccessor = timeAccessor;

    if(type != LINEAR)
        LOGE("Type %s not supported, using LINEAR\n", type == STEP ? "STEP" : "CUBICSPLINE");
    this->type = type;
}

void Vec3Animation::update(float deltaT)
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
    {
        LOGE("Interpolation value was %f\n", interpolationValue);
        LOGI("Index %d, Previous time %f, current time %f, next time %f\n", currentKeyframeIndex, previousTime, currentTime, nextTime);
    }
    
    Transform t = target.getTransform();

    if(path == TRANSLATION)
        t.translation = glm::mix((*dataAccessor)[currentKeyframeIndex - 1], (*dataAccessor)[currentKeyframeIndex], interpolationValue);
    else if(path == SCALE)
        t.scale = glm::mix((*dataAccessor)[currentKeyframeIndex - 1], (*dataAccessor)[currentKeyframeIndex], interpolationValue);

    target.setTransform(t);
}