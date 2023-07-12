#ifndef V_ATTACHABLE_HPP
#define V_ATTACHABLE_HPP

class Node;

class Attachable
{
    public:
    void setNode(Node *n);
    virtual void update();
    virtual ~Attachable() = 0;
    Node *getNode();

    private:
    Node *node = nullptr;
};

#endif