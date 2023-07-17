#include "scene/v_attachable.hpp"

void Attachable::setNode(Node *n)
{
    this->node = n;
}

Node *Attachable::getNode()
{
    return this->node;
}

Attachable::~Attachable() = default;
