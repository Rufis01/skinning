#ifndef V_ATTACHABLE_HPP
#define V_ATTACHABLE_HPP

#include "scene/v_node_observer.hpp"

class Node;

class Attachable
{
    public:
    void setNode(Node *n);
    virtual ~Attachable() = 0;
    Node *getNode();

    private:
    Node *node = nullptr;
};

#endif