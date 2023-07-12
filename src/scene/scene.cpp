#include "scene/scene.hpp"

Scene::Scene() = default;

Scene::Scene(std::set<Node *> rootNodes) : rootNodes(rootNodes)
{
}

void Scene::addNode(Node *node)
{
    nodes.emplace(node);
}

void Scene::addMesh(Mesh *mesh)
{
    meshes.emplace(mesh);
}

void Scene::addCamera(Camera *camera)
{
    cameras.emplace(camera);
}

void Scene::addSkin(Skin *skin)
{
    skins.emplace(skin);
}

void Scene::addLight(Light *light)
{
    lights.emplace(light);
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
