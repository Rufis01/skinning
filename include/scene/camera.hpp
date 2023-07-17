#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/glm.hpp"

#include "v_attachable.hpp"

class Camera : public Attachable, public NodeObserver
{
    public:
    Camera(glm::mat4 &projectionMatrix);

    glm::mat4 &getViewMatrix();         //E^-1  (From World space to Camera space)
    glm::mat4 &getProjectionMatrix();   //(From Camera space to Clip space)
    glm::mat4 &getVPMatrix();           //Projection * View

    virtual void update(Node *node);              //Lazy updating. Recalculate matrices only when needed.
    void doUpdate();                  

    //void setProjectionMatrix();

    //virtual ~Camera() = 0;

    private:
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 VPMatrix;
    bool isDirty = true;             //Do I need to recalculate the matrices?
    void recalculateViewMatrix();
    void recalculateVPMatrix();
};

#endif
