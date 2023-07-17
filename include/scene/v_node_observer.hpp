#ifndef V_NODE_OBSERVER_HPP
#define V_NODE_OBSERVER_HPP

class Node;

class NodeObserver
{
    public:
    virtual void update(Node *n);
    virtual ~NodeObserver() = 0;
};

#endif
