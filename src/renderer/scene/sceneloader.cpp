#include <string>

#include "renderer/scene/sceneloader.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_USE_CPP14
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include "tiny_gltf.h"
#pragma GCC diagnostic pop

#include <assert.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "log.h"
#include "renderer/accessor/accessor.hpp"

static void loadRootNodes(Scene &myScene, tinygltf::Scene &scene, tinygltf::Model &model, std::vector<Node *> &nodes);
static Node *loadNode(Scene &myScene, tinygltf::Model &model, int index);
static Camera *loadCamera(Scene &myScene, tinygltf::Model &model, int index);
static Skin *loadSkin(Scene &myScene, tinygltf::Model &model, int index);
static Mesh *loadMesh(Scene &myScene, tinygltf::Model &model, int index);
static Light *loadLight(Scene &myScene, tinygltf::Model &model, int index);

template <typename T>
static Accessor<T> getAccessor(tinygltf::Model &model, int index);

void loadScene(Scene &scene, const char *path, int index)
{
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;

    std::string warn, err;

    bool res = loader.LoadASCIIFromFile(&model, &err, &warn, path);

    if(!warn.empty())
        LOGW("TinyGLTF: %s", warn.c_str());
        
    if(!err.empty())
        LOGE("TinyGLTF: %s", err.c_str());

    if(!res)
    {
        throw std::runtime_error("TinyGLTF failed to load the file");
    }

    tinygltf::Scene &s = model.scenes[index];
    std::vector<Node *> rootNodes;
    loadRootNodes(scene, s, model, rootNodes);
}

void loadRootNodes(Scene &myScene, tinygltf::Scene &scene, tinygltf::Model &model, std::vector<Node *> &nodes)
{
    ///TODO: maybe load everything instead of just what used for the first scene?
    std::vector<Node *> rootNodes;
    for(int i : scene.nodes)
    {

        Node *myNode = loadNode(myScene, model, i);

        rootNodes.push_back(myNode);
    }
}

Node *loadNode(Scene &myScene, tinygltf::Model &model, int index)
{
    tinygltf::Node n = model.nodes[index];
    std::vector<Node *> children;


    for(int i : n.children)
    {
        children.push_back(loadNode(myScene, model, i));
    }

    Transform t;

    if(n.matrix.size() == 0)
    {
        t.rotation = n.rotation.size() != 0 ? glm::quat(n.rotation[0], n.rotation[1], n.rotation[2], n.rotation[3]) : glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        t.scale = n.scale.size() != 0 ? glm::vec3(n.scale[0], n.scale[1], n.scale[2]) : glm::vec3(1.0f);
        t.translation = n.translation.size() != 0 ? glm::vec3(n.translation[0], n.translation[1], n.translation[2]) : glm::vec3(0.0f);
    }
    else
    {
        std::vector<float> mat(n.matrix.begin(), n.matrix.end());
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(glm::make_mat4(mat.data()), t.scale, t.rotation, t.translation, skew, perspective);
    }

    return new Node(children, t,
                    n.camera != -1 ? loadCamera(myScene, model, n.camera) : nullptr,
                    n.skin   != -1 ? loadSkin  (myScene, model, n.skin  ) : nullptr,
                    n.mesh   != -1 ? loadMesh  (myScene, model, n.mesh  ) : nullptr,
                    n.light  != -1 ? loadLight (myScene, model, n.light ) : nullptr);

}

Camera *loadCamera(Scene &myScene, tinygltf::Model &model, int index)
{
    tinygltf::Camera c = model.cameras[index];
    glm::mat4 projMat;
    
    if(c.type == "perspective")
    {
        projMat = glm::perspective(c.perspective.yfov, c.perspective.aspectRatio,
                                   c.perspective.znear, c.perspective.zfar);
    }
    else if(c.type == "orthogonal")
    {
        projMat = glm::ortho(-c.orthographic.xmag/2, c.orthographic.xmag/2,
                             -c.orthographic.ymag/2, -c.orthographic.ymag/2);
    }
    else
    {
        LOGF("Unknown camera type");
        throw std::runtime_error("Unknown camera type");
    }

    return nullptr;
}

Skin *loadSkin(Scene &myScene, tinygltf::Model &model, int index)
{
    tinygltf::Skin s = model.skins[index];

    return nullptr;
}

Mesh *loadMesh(Scene &myScene, tinygltf::Model &model, int index)
{
    tinygltf::Mesh m = model.meshes[index];
    std::vector<Primitive> primitives;

    for(tinygltf::Primitive p : m.primitives)
    {
        int pos = p.attributes.contains("POSITION") ? p.attributes["POSITION"] : -1;
        //int nor = p.attributes.contains("NORMAL") ? p.attributes["NORMAL"] : -1;
        //int tex = p.attributes.contains("TEX_COORD0") ? p.attributes["TEX_COORD0"] : -1;
        int idx = p.indices;

        Accessor<glm::vec3> pa = getAccessor<glm::vec3>(model, pos);
        //Accessor<glm::vec3> na = getAccessor<glm::vec3>(model, nor);
        //Accessor<glm::vec2> ta = getAccessor<glm::vec2>(model, tex);
        Accessor<short> pi = getAccessor<short>(model, idx);

        //assert(pa.getCount() == na.getCount() && na.getCount() == ta.getCount());//pos nor and tex should all be the same size

        std::vector<Vertex> vertices;
        std::vector<short> indicies;
        vertices.resize(pa.getCount());

        for(int i = 0; i < pa.getCount(); i++)
        {
            Vertex v;
            v.position = pa[i];
            //v.normal = na[i];
            //v.uv = ta[i];
            vertices.push_back(v);
        }

        for(int i : pi)
        {
            indicies.push_back(i);
        }

        primitives.push_back(Primitive(vertices, indicies));
    }

    return new Mesh(primitives);
}

Light *loadLight(Scene &myScene, tinygltf::Model &model, int index)
{
    tinygltf::Light l = model.lights[index];

    return nullptr;
}

template <typename T>
Accessor<T> getAccessor(tinygltf::Model &model, int index)
{
    tinygltf::Accessor a = model.accessors[index];
    tinygltf::BufferView b = model.bufferViews[a.bufferView];

    BufferView bv = BufferView((long)b.byteOffset, (long)b.byteLength, (long)b.byteStride, model.buffers[b.buffer].data);
    return Accessor<T>(bv, (long)a.byteOffset, (long)a.count);
}
