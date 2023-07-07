#include "renderer/scene/camera.hpp"
#include "renderer/scene/node.hpp"

Camera::Camera(glm::mat4 &projectionMatrix) : projectionMatrix(projectionMatrix)
{
}

glm::mat4 &Camera::getViewMatrix()
{
    if(!isUpdated)
        doUpdate();
    return this->viewMatrix;
}

glm::mat4 &Camera::getProjectionMatrix()
{
    return this->projectionMatrix;
}

glm::mat4 &Camera::getVPMatrix()
{
    if(!isUpdated)
        doUpdate();
    return this->VPMatrix;
}


void Camera::update()
{
    isUpdated = false;
}

void Camera::doUpdate()
{
    recalculateViewMatrix();
    recalculateVPMatrix();
}   


//void setProjectionMatrix();

void Camera::recalculateViewMatrix()
{
    this->viewMatrix = glm::inverse(this->getNode()->getModelMatrix());
}

void Camera::recalculateVPMatrix()
{
    VPMatrix = projectionMatrix * viewMatrix;
}
