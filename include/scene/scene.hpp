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
    Scene(std::vector<Node *> rootNodes);
    ~Scene();

    void addNode  (Node   *node  );
    void addMesh  (Mesh   *mesh  );
    void addCamera(Camera *camera);
    void addSkin  (Skin   *skin  );
    void addLight (Light  *light );
    
    std::vector<Mesh   *> meshes;
    std::vector<Camera *> cameras;
    std::vector<Node   *> nodes;
    std::vector<Skin   *> skins;
    std::vector<Light  *> lights;

    private:

    //std::vector<Animation> animations;
    std::vector<Node   *> rootNodes;
    //std::vector<Texture> textures;
    //std::vector<Sampler> samplers;
};

#endif
