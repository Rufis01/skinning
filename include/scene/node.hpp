#ifndef NODE_HPP
#define NODE_HPP

#include <vector>

#include "camera.hpp"
#include "mesh.hpp"
#include "light.hpp"
#include "skin.hpp"

#include "transform.hpp"

#include "glm/glm.hpp"

class Node
{
    public:
    Node(std::vector<Node *> &children, Transform &trans, Camera *camera, Skin *skin, Mesh *mesh, Light*light);
    void setTransform(Transform &transform);
    Transform &getTransform();
    glm::mat4 &getGlobalModelMatrix();
    ~Node() = default;

    private:
    bool isDirty = true;
    Node *parent = nullptr;

    std::vector<Node *> &children;
    
    Transform transform;

    Camera *camera = nullptr;
    Skin   *skin   = nullptr;
    Mesh   *mesh   = nullptr;
    Light  *light  = nullptr;


    glm::mat4 localModelMatrix;     //(From Object space to Parent space)
    glm::mat4 globalModelMatrix;    //(From Object space to World space)

    glm::mat4 &getLocalModelMatrix();
    void setParent(Node *n);
    void notify();
    void update();
    void doUpdate();
};

#endif
