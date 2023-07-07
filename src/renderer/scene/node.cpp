#include "renderer/scene/node.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

Node::Node(std::vector<Node *> &children, Transform &trans, Camera *camera, Skin *skin, Mesh *mesh, Light*light) :
    children(children),
    transform(trans),
    camera(camera),
    skin(skin),
    mesh(mesh),
    light(light)
{
}

void Node::setTransform(Transform &transform)
{
    this->transform = transform;
    this->modelMatrix = glm::identity<glm::mat4>();
    this->modelMatrix = glm::scale(modelMatrix, transform.scale);
    this->modelMatrix *= glm::mat4_cast(transform.rotation);
    this->modelMatrix = glm::translate(modelMatrix, transform.translation);
}

Transform &Node::getTransform()
{
    return this->transform;
}

glm::mat4 &Node::getModelMatrix()
{
    return this->modelMatrix;
}


void Node::setParent(Node *n)
{
    this->parent = n;
}

void Node::notify()
{
    if(camera) camera->update();
    if(skin  ) skin  ->update();
    if(mesh  ) mesh  ->update();
    if(light ) light ->update();
}
