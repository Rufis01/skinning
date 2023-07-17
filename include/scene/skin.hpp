#ifndef SKIN_HPP
#define SKIN_HPP

#include "v_attachable.hpp"

#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>

class Skin : public Attachable, public NodeObserver
{
    public:
    Skin(std::vector<glm::mat4> &inverseBindMatrices);
    void setJoints(std::vector<Node *> joints);
    void uploadToGPU();
    virtual void update(Node *n);
    void doUpdate();
    void bind();
    std::vector<Node *> joints;
    void forceUpdate();

    private:
    GLuint jointMatricesHandle;
    std::vector<glm::mat4> inverseBindMatrices;
};

#endif