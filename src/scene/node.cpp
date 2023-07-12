#include "scene/node.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

Node::Node(std::vector<Node *> &children, Transform &trans, Camera *camera, Skin *skin, Mesh *mesh, Light*light) :
    children(children),
    camera(camera),
    skin(skin),
    mesh(mesh),
    light(light)
{
    setTransform(trans);
    for(Node *n : children)
        n->setParent(this);
    if(camera) camera->setNode(this);
    if(skin  ) skin  ->setNode(this);
    if(mesh  ) mesh  ->setNode(this);
    if(light ) light ->setNode(this);
}

void Node::setTransform(Transform &transform)
{
    this->transform = transform;
    this->localModelMatrix = glm::identity<glm::mat4>();
    this->localModelMatrix = glm::translate(localModelMatrix, transform.translation);
    this->localModelMatrix = glm::scale(localModelMatrix, transform.scale);
    this->localModelMatrix *= glm::mat4_cast(transform.rotation);
    isDirty = true;
}

Transform &Node::getTransform()
{
    return this->transform;
}

glm::mat4 &Node::getGlobalModelMatrix()
{
    if(isDirty)
        doUpdate();
    return this->globalModelMatrix;
}

glm::mat4 &Node::getLocalModelMatrix()
{
    return this->localModelMatrix;
}


void Node::setParent(Node *n)
{
    this->parent = n;
}

void Node::notify()
{
    for(Node *n : children)
        n->update();
    if(camera) camera->update();
    if(skin  ) skin  ->update();
    if(mesh  ) mesh  ->update();
    if(light ) light ->update();
}

void Node::update()
{
    isDirty = true;
    notify();
}

void Node::doUpdate()
{
    if(parent)
        this->globalModelMatrix = parent->getGlobalModelMatrix() * this->getLocalModelMatrix();
    else
        this->globalModelMatrix = this->getLocalModelMatrix();
    isDirty = false;
}