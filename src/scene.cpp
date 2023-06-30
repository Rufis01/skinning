#include <stdexcept>

#include "scene.hpp"

#include "log.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_USE_CPP14
#include "tiny_gltf.h"

Scene::Scene(){}

Scene::Scene(std::string path)
{
    tinygltf::TinyGLTF loader;

    std::string warn, err;

    bool res = loader.LoadASCIIFromFile(&scene, &err, &warn, path);

    if(!warn.empty())
        LOGW("TinyGLTF: %s", warn.c_str());
        
    if(!err.empty())
        LOGE("TinyGLTF: %s", err.c_str());

    if(!res)
        throw std::runtime_error("TinyGLTF failed to load the file");
        
}

Scene::~Scene()
{

}