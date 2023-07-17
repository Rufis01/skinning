#include "scene/scene.hpp"

Scene::Scene() = default;

Scene::Scene(std::vector<Node *> rootNodes) : rootNodes(rootNodes)
{
}

void Scene::addNode(Node *node)
{
    nodes.emplace_back(node);
}

void Scene::addMesh(Mesh *mesh)
{
    meshes.emplace_back(mesh);
}

void Scene::addCamera(Camera *camera)
{
    cameras.emplace_back(camera);
}

void Scene::addSkin(Skin *skin)
{
    skins.emplace_back(skin);
}

void Scene::addLight(Light *light)
{
    lights.emplace_back(light);
}

Scene::~Scene()
{
    for(auto e : meshes)
        delete e;
    for(auto e : lights)
        delete e;
    for(auto e : cameras)
        delete e;
    for(auto e : skins)
        delete e;
    for(auto e : nodes)
        delete e;
    for(auto e : rootNodes)
        delete e;
}
