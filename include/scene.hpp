#ifndef SCENE_HPP
#define SCENE_HPP

#include "tiny_gltf.h"

class Scene
{
    public:
    Scene();
    Scene(std::string path);
    ~Scene();

    private:
    tinygltf::Model scene;
};

#endif
