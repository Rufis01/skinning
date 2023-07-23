#include "scene/camera.hpp"
#include "scene/node.hpp"

#include "log.h"

Camera::Camera(glm::mat4 &projectionMatrix) : projectionMatrix(projectionMatrix)
{
    LOGD("Camera loaded\n");
}

glm::mat4 &Camera::getViewMatrix()
{
    if(isDirty)
        doUpdate();
    return this->viewMatrix;
}

glm::mat4 &Camera::getProjectionMatrix()
{
    return this->projectionMatrix;
}

glm::mat4 &Camera::getVPMatrix()
{
    if(isDirty)
        doUpdate();
    return this->VPMatrix;
}


void Camera::update(Node *n)
{
    isDirty = true;
}

void Camera::doUpdate()
{
    recalculateViewMatrix();
    recalculateVPMatrix();
    isDirty = false;
}   

void Camera::recalculateViewMatrix()
{
    this->viewMatrix = glm::inverse(this->getNode()->getGlobalModelMatrix());
}

void Camera::recalculateVPMatrix()
{
    VPMatrix = projectionMatrix * viewMatrix;
}
