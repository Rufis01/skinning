#include "scene/skin.hpp"
#include "scene/node.hpp"

#include "log.h"

Skin::Skin(std::vector<glm::mat4> &inverseBindMatrices) : inverseBindMatrices(inverseBindMatrices)
{
}

void Skin::setJoints(std::vector<Node *> joints)
{
    this->joints = joints;
    for(Node *j : joints)
    {
        j->attachObserver(this);
    }
}

void Skin::uploadToGPU()
{
    glGenBuffers(1, &jointMatricesHandle);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, jointMatricesHandle);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * joints.size(), nullptr, GL_DYNAMIC_DRAW);

    for(unsigned int i = 0; i < joints.size(); i++)
    {
        glm::mat4 jointMat = joints[i]->getGlobalModelMatrix() * inverseBindMatrices[i];
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * i, sizeof(glm::mat4) , &jointMat);
    }

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, jointMatricesHandle);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Skin::update(Node *n)
{
    int idx = -1;
    for(unsigned int i = 0; i < joints.size(); i++)
    {
        if(joints[i] == n) idx = i;
    }

    if(idx == -1)
    {
        LOGW("Skin was notified by node %s, which is not a joint.\n", n->name.c_str());
        return;
    }

    //LOGD("Updating joint %i\n", idx);
    glm::mat4 jointMat = joints[idx]->getGlobalModelMatrix() * inverseBindMatrices[idx];

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, jointMatricesHandle);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * idx, sizeof(glm::mat4) , &jointMat);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Skin::bind()
{
    //LOGD("Binding skin\n");
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, jointMatricesHandle);
}

void Skin::forceUpdate()
{
    for(Node *j : joints)
    {
        update(j);
    }
}