#ifndef SCENE_HPP
#define SCENE_HPP

#include <set>

#include "mesh.hpp"
#include "node.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "skin.hpp"

class Scene
{
    public:
    Scene();
    Scene(std::set<Node *> rootNodes);
    ~Scene();

    void addNode  (Node   *node  );
    void addMesh  (Mesh   *mesh  );
    void addCamera(Camera *camera);
    void addSkin  (Skin   *skin  );
    void addLight (Light  *light );
    
    std::set<Mesh   *> meshes;
    std::set<Camera *> cameras;

    private:

    //std::vector<Animation> animations;
    std::set<Node   *> rootNodes;
    std::set<Node   *> nodes;
    std::set<Skin   *> skins;
    std::set<Light  *> lights;
    //std::vector<Texture> textures;
    //std::vector<Sampler> samplers;
};

#endif
