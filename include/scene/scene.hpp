#ifndef SCENE_HPP
#define SCENE_HPP

#include <set>

#include "mesh/mesh.hpp"
#include "node.hpp"
#include "camera.hpp"
#include "skin.hpp"
#include "animation/animation.hpp"
#include "mesh/texture.hpp"

class Scene
{
    public:
    Scene();
    Scene(std::vector<Node *> rootNodes);
    ~Scene();

    void addNode     (Node        *node   );
    void addMesh     (Mesh        *mesh   );
    void addCamera   (Camera      *camera );
    void addSkin     (Skin        *skin   );
    void addAnimation(Animation   *anim   );
    void addTexture  (Texture     *texture);
    
    std::vector<Mesh      *> meshes;
    std::vector<Camera    *> cameras;
    std::vector<Node      *> nodes;
    std::vector<Skin      *> skins;
    std::vector<Animation *> animations;
    std::vector<Texture   *> textures;
    
    std::vector<std::vector<unsigned char>> buffers;

    private:

    std::vector<Node *> rootNodes;
};

#endif
