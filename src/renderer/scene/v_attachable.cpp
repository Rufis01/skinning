#include "renderer/scene/v_attachable.hpp"

void Attachable::setNode(Node *n)
{
    this->node = n;
}

void Attachable::update()
{
}


Node *Attachable::getNode()
{
    return this->node;
}

Attachable::~Attachable() = default;
