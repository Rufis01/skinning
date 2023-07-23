#include "scene/node.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "log.h"

Node::Node(std::vector<Node *> &children, Transform &trans, Camera *camera, Skin *skin, Mesh *mesh, std::string name) :
    name(name),
    skin(skin),
    children(children),
    camera(camera),
    mesh(mesh)
{
    for(Node *n : children)
        n->setParent(this);
    if(camera) camera->setNode(this);
    if(skin  ) skin  ->setNode(this);
    if(mesh  ) mesh  ->setNode(this);
    setTransform(trans);
}

void Node::setTransform(Transform &transform)
{
    this->transform = Transform(transform);
    doUpdateLocalMatrix();
    localMatrixIsDirty = true;
    globalMatrixIsDirty = true;
    notify();
}

Transform &Node::getTransform()
{
    return this->transform;
}

glm::mat4 &Node::getGlobalModelMatrix()
{
    if(globalMatrixIsDirty)
        doUpdateGlobalMatrix();
    return this->globalModelMatrix;
}

glm::mat4 &Node::getLocalModelMatrix()
{
    if(localMatrixIsDirty)
        doUpdateLocalMatrix();
    return this->localModelMatrix;
}


void Node::setParent(Node *n)
{
    this->parent = n;
}

void Node::notify()
{
    for(Node *n : children)
        n->update(this);

    for(NodeObserver *o : observers)
        o->update(this);
        
    if(camera) camera->update(this);
    if(skin  ) skin  ->update(this);
    if(mesh  ) mesh  ->update(this);
}

void Node::update(Node *n)
{
    globalMatrixIsDirty = true;
    notify();
}

void Node::doUpdateLocalMatrix()
{
    this->localModelMatrix = glm::identity<glm::mat4>();
    this->localModelMatrix = glm::translate(localModelMatrix, this->transform.translation);
    this->localModelMatrix = localModelMatrix * glm::mat4_cast(this->transform.rotation);
    this->localModelMatrix = glm::scale(localModelMatrix, this->transform.scale);

    localMatrixIsDirty = false;
}

void Node::doUpdateGlobalMatrix()
{
    if(parent)
        this->globalModelMatrix = parent->getGlobalModelMatrix() * this->getLocalModelMatrix();
    else
        this->globalModelMatrix = this->getLocalModelMatrix();
    globalMatrixIsDirty = false;

    //LOGD("Node %s updated its matrix. %s%s\n", name.c_str(), parent ? "Its parent is " : "", parent ? parent->name.c_str() : "");
    //glm::vec3 euler = glm::eulerAngles(this->transform.rotation) * 180.0f / 3.14159f;
    //LOGD("Node %s has rotation X:%f Y:%f Z:%f\n", this->name.c_str(), euler.x, euler.y, euler.z);
}


void Node::attachObserver(NodeObserver *obs)
{
    observers.push_back(obs);
}