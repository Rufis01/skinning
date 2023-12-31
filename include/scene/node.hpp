#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <string>

#include "camera.hpp"
#include "scene/mesh/mesh.hpp"
#include "skin.hpp"

#include "transform.hpp"

#include "glm/glm.hpp"

#include "scene/v_node_observer.hpp"

class Node : NodeObserver
{
    public:
    Node(std::vector<Node *> &children, Transform &trans, Camera *camera, Skin *skin, Mesh *mesh, std::string name);
    void setTransform(Transform &transform);
    Transform &getTransform();
    glm::mat4 &getGlobalModelMatrix();
    ~Node() = default;
    std::string name;
    Skin   *skin   = nullptr;

    void attachObserver(NodeObserver *obs);

    private:
    bool localMatrixIsDirty = true;
    bool globalMatrixIsDirty = true;
    Node *parent = nullptr;

    std::vector<Node *> children;
    std::vector<NodeObserver *> observers;
    
    Transform transform;

    Camera *camera = nullptr;
    Mesh   *mesh   = nullptr;


    glm::mat4 localModelMatrix;     //(From Object space to Parent space)
    glm::mat4 globalModelMatrix;    //(From Object space to World space)

    glm::mat4 &getLocalModelMatrix();
    void setParent(Node *n);
    void notify();
    virtual void update(Node *n);
    void doUpdateLocalMatrix();
    void doUpdateGlobalMatrix();
};

#endif
