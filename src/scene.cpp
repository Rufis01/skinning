#include "scene.hpp"

#include "log.h"
#include "tiny_gltf.h"


Scene::Scene()
{
    tinygltf::TinyGLTF loader;

    std::string warn, err;

    bool res = loader.LoadASCIIFromFile(&scene, &err, &warn, "PaperinoPippoEPluto.gltf");

    if(!warn.empty())
        LOGW("TinyGLTF: %s", warn.c_str());
        
    if(!err.empty())
        LOGE("TinyGLTF: %s", err.c_str());

    if(!res)
        throw std::runtime_error("TinyGLTF failed to load the file");
        
}