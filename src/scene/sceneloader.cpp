#include <string>

#include "scene/sceneloader.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_USE_CPP14
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include "tiny_gltf.h"
#pragma GCC diagnostic pop

#include <typeinfo>
#include <assert.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "log.h"
#include "accessor/accessor.hpp"

static void loadRootNodes(Scene &myScene, tinygltf::Scene &scene, tinygltf::Model &model, std::vector<Node *> &nodes);
static Node *loadNode(Scene &myScene, tinygltf::Model &model, tinygltf::Node);
static Camera *loadCamera(tinygltf::Model &model, tinygltf::Camera c);
static Skin *loadSkin(tinygltf::Model &model, tinygltf::Skin s);
static void setSkinJoints(Scene &myScene, Skin *mySkin, tinygltf::Skin s);
static Mesh *loadMesh(tinygltf::Model &model, tinygltf::Mesh m);
static Light *loadLight(tinygltf::Model &model, tinygltf::Light l);

template <typename T>
static Accessor<T> getAccessor(tinygltf::Model &model, int index);

void loadScene(Scene &scene, const char *path, int index)
{
    ///TODO: maybe reorder the mesh array based on the skin index. We don't want to swap skins everytime.
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
    for(tinygltf::Camera c : model.cameras)
    {
        myScene.addCamera(loadCamera(model, c));
    }

    for(tinygltf::Mesh m : model.meshes)
    {
        myScene.addMesh(loadMesh(model, m));
    }

    for(tinygltf::Skin s : model.skins)
    {
        myScene.addSkin(loadSkin(model, s));
    }

    for(tinygltf::Light l : model.lights)
    {
        myScene.addLight(loadLight(model, l));
    }

    myScene.nodes.resize(model.nodes.size(), nullptr);
    for(int i=0; i<model.nodes.size(); i++)
    {
        if(myScene.nodes[i] != nullptr) continue;
        Node *n = loadNode(myScene, model, model.nodes[i]);
        myScene.nodes[i] = n;
    }

    for(int i = 0; i < model.skins.size(); i++)
    {
        setSkinJoints(myScene, myScene.skins[i], model.skins[i]);
    }

    ///TODO: maybe load everything instead of just what used for the first scene?
    std::vector<Node *> rootNodes;
    for(int i : scene.nodes)
    {
        rootNodes.push_back(myScene.nodes[i]);
    }
}

Node *loadNode(Scene &myScene, tinygltf::Model &model, tinygltf::Node n)
{
    std::vector<Node *> children;

    //LOGD("Children of %s:\n", n.name.c_str());
    for(int i : n.children)
    {
        Node *c;
        if(myScene.nodes[i] != nullptr)
        {
            c = myScene.nodes[i];
        }
        else
        {
            c = loadNode(myScene, model, model.nodes[i]);
            myScene.nodes[i] = c;
        }
        //LOGD("\t%s\n", c->name.c_str());
        children.push_back(c);
    }

    Transform t;

    if(n.matrix.size() == 0)
    {
        t.rotation = n.rotation.size() != 0 ? glm::quat(n.rotation[3], n.rotation[0], n.rotation[1], n.rotation[2]) : glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        t.scale = n.scale.size() != 0 ? glm::vec3(n.scale[0], n.scale[1], n.scale[2]) : glm::vec3(1.0f, 1.0f, 1.0f);
        t.translation = n.translation.size() != 0 ? glm::vec3(n.translation[0], n.translation[1], n.translation[2]) : glm::vec3(0.0f, 0.0f, 0.0f);
    }
    else
    {
        std::vector<float> mat(n.matrix.begin(), n.matrix.end());
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(glm::make_mat4(mat.data()), t.scale, t.rotation, t.translation, skew, perspective);
    }

    Node* node = new Node(children, t,
                          n.camera != -1 ? myScene.cameras[n.camera] : nullptr,
                          n.skin   != -1 ? myScene.skins  [n.skin  ] : nullptr,
                          n.mesh   != -1 ? myScene.meshes [n.mesh  ] : nullptr,
                          n.light  != -1 ? myScene.lights [n.light ] : nullptr,
                          n.name);
    return node;
}

Camera *loadCamera(tinygltf::Model &model, tinygltf::Camera c)
{
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

    Camera *camera = new Camera(projMat);

    return camera;
}

Skin *loadSkin(tinygltf::Model &model, tinygltf::Skin s)
{
    std::vector<glm::mat4> inverseBindMatrices;

    Accessor<glm::mat4> matrices = getAccessor<glm::mat4>(model, s.inverseBindMatrices);

    for(glm::mat4 mat : matrices)
    {
        inverseBindMatrices.push_back(mat);
    }

    return new Skin(inverseBindMatrices);
}

void setSkinJoints(Scene &myScene, Skin *mySkin, tinygltf::Skin s)
{
    std::vector<Node *> joints;
    for(int i : s.joints)
    {
        joints.push_back(myScene.nodes[i]);
    }

    mySkin->setJoints(joints);
    
}


namespace glm
{
    typedef vec<4, unsigned char, defaultp> ubvec4;
};
Mesh *loadMesh(tinygltf::Model &model, tinygltf::Mesh m)
{
    std::vector<Primitive> primitives;

    for(tinygltf::Primitive p : m.primitives)
    {
        int pos = p.attributes.contains("POSITION") ? p.attributes["POSITION"] : -1;
        int nor = p.attributes.contains("NORMAL") ? p.attributes["NORMAL"] : -1;
        int tex = p.attributes.contains("TEXCOORD_0") ? p.attributes["TEXCOORD_0"] : -1;
        int jnt = p.attributes.contains("JOINTS_0") ? p.attributes["JOINTS_0"] : -1;
        int wgh = p.attributes.contains("WEIGHTS_0") ? p.attributes["WEIGHTS_0"] : -1;
        int idx = p.indices;

        Accessor<glm::vec3> pa = getAccessor<glm::vec3>(model, pos);
        Accessor<glm::vec3> na = getAccessor<glm::vec3>(model, nor);
        Accessor<glm::vec2> ta = getAccessor<glm::vec2>(model, tex);
        Accessor<glm::ubvec4> ja = getAccessor<glm::ubvec4>(model, jnt);
        Accessor<glm::vec4> wa = getAccessor<glm::vec4>(model, wgh);
        Accessor<short> pi = getAccessor<short>(model, idx);

        //assert(pa.getCount() == na.getCount() && na.getCount() == ta.getCount());//pos nor and tex should all be the same size

        std::vector<Vertex> vertices;
        std::vector<short> indicies;
        vertices.reserve(pa.getCount());
        indicies.reserve(pi.getCount());
        LOGD("%d\n", pa.getCount());

        for(int i = 0; i < pa.getCount(); i++)
        {
            glm::ubvec4 j = ja[i];
            glm::vec4 joints = glm::vec4((uint)j.x, (uint)j.y, (uint)j.z, (uint)j.w);
            Vertex v = {pa[i], na[i], ta[i], joints, wa[i]};
            vertices.push_back(v);
        }

        for(short i : pi)
        {
            indicies.push_back(i);
        }

        primitives.push_back(Primitive(vertices, indicies));
    }

    Mesh *mesh = new Mesh(primitives);
    return mesh;
}

Light *loadLight(tinygltf::Model &model, tinygltf::Light l)
{

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
