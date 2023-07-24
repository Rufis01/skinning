#include <string>

#include "sceneloader.hpp"

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

#include "scene/animation/quatAnimation.hpp"
#include "scene/animation/vec3Animation.hpp"

typedef unsigned int uint;

static void loadRootNodes(Scene &myScene, tinygltf::Scene &scene, tinygltf::Model &model, std::vector<Node *> &nodes);
static Node *loadNode(Scene &myScene, tinygltf::Model &model, tinygltf::Node &n);
static Camera *loadCamera(tinygltf::Model &model, tinygltf::Camera &c);
static Skin *loadSkin(Scene &myScene, tinygltf::Model &model, tinygltf::Skin &s);
static void setSkinJoints(Scene &myScene, Skin *mySkin, tinygltf::Skin &s);
static Mesh *loadMesh(Scene &myScene, tinygltf::Model &model, tinygltf::Mesh &m);
static std::vector<Animation *> loadAnimation(Scene &myScene, tinygltf::Model &model, tinygltf::Animation &a);
static Texture *loadTexture(tinygltf::Model &model, tinygltf::Texture &t);

template <typename T>
static Accessor<T> *getAccessor(Scene &myScene, tinygltf::Model &model, int index);

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

static void loadRootNodes(Scene &myScene, tinygltf::Scene &scene, tinygltf::Model &model, std::vector<Node *> &nodes)
{
    myScene.buffers.resize(model.buffers.size());
    for(unsigned int i = 0; i < model.buffers.size(); i++)
    {
        myScene.buffers[i] = model.buffers[i].data;
    }

    for(tinygltf::Camera c : model.cameras)
    {
        myScene.addCamera(loadCamera(model, c));
    }

    myScene.textures.resize(model.textures.size());
    for(unsigned int i = 0; i < model.textures.size(); i++)
    {
        myScene.textures[i] = loadTexture(model, model.textures[i]);
    }

    for(tinygltf::Mesh m : model.meshes)
    {
        myScene.addMesh(loadMesh(myScene, model, m));
    }

    for(tinygltf::Skin s : model.skins)
    {
        myScene.addSkin(loadSkin(myScene, model, s));
    }

    myScene.nodes.resize(model.nodes.size(), nullptr);
    for(unsigned int i=0; i<model.nodes.size(); i++)
    {
        if(myScene.nodes[i] != nullptr) continue;
        Node *n = loadNode(myScene, model, model.nodes[i]);
        myScene.nodes[i] = n;
    }

    for(tinygltf::Animation a : model.animations)
    {
        std::vector<Animation *> anims = loadAnimation(myScene, model, a);
        for(Animation *ap : anims)
            myScene.addAnimation(ap);
    }

    for(unsigned int i = 0; i < model.skins.size(); i++)
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

static Node *loadNode(Scene &myScene, tinygltf::Model &model, tinygltf::Node &n)
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
                          n.name);
    return node;
}

static Camera *loadCamera(tinygltf::Model &model, tinygltf::Camera &c)
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

static Skin *loadSkin(Scene &myScene, tinygltf::Model &model, tinygltf::Skin &s)
{
    std::vector<glm::mat4> inverseBindMatrices;

    Accessor<glm::mat4> *matrices = getAccessor<glm::mat4>(myScene, model, s.inverseBindMatrices);

    for(glm::mat4 mat : *matrices)
    {
        inverseBindMatrices.push_back(mat);
    }

    return new Skin(inverseBindMatrices);
}

static void setSkinJoints(Scene &myScene, Skin *mySkin, tinygltf::Skin &s)
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
static Mesh *loadMesh(Scene &myScene, tinygltf::Model &model, tinygltf::Mesh &m)
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

        Accessor<glm::vec3> *pa = getAccessor<glm::vec3>(myScene, model, pos);
        Accessor<glm::vec3> *na = getAccessor<glm::vec3>(myScene, model, nor);
        Accessor<glm::vec2> *ta = getAccessor<glm::vec2>(myScene, model, tex);
        Accessor<glm::ubvec4> *ja = getAccessor<glm::ubvec4>(myScene, model, jnt);
        Accessor<glm::vec4> *wa = getAccessor<glm::vec4>(myScene, model, wgh);
        Accessor<short> *pi = getAccessor<short>(myScene, model, idx);

        //assert(pa.getCount() == na.getCount() && na.getCount() == ta.getCount());//pos nor and tex should all be the same size

        std::vector<Vertex> vertices;
        std::vector<short> indicies;
        vertices.reserve(pa->getCount());
        indicies.reserve(pi->getCount());
        LOGD("Primitive has %d vertices\n", pa->getCount());

        /*for(int i = 0; i < pa->getCount(); i++)
        {
            glm::ubvec4 j = (*ja)[i];
            glm::vec4 joints = glm::vec4((uint)j.x, (uint)j.y, (uint)j.z, (uint)j.w);
            Vertex v = {*pa[i], *na[i], ta[i], joints, wa[i]};
            vertices.push_back(v);
        }*/
        
        for(int i = 0; i < pa->getCount(); i++)
        {
            glm::ubvec4 j = ja ? (*ja)[i] : glm::ubvec4(0,0,0,0);
            glm::vec4 joints = glm::vec4((uint)j.x, (uint)j.y, (uint)j.z, (uint)j.w);

            Vertex v;
            v.joints = joints;
            v.position = pa ? (*pa)[i] : glm::vec3(0.0f);
            v.normal   = na ? (*na)[i] : glm::vec3(0.0f);
            v.uv       = ta ? (*ta)[i] : glm::vec2(0.0f);
            v.weights  = wa ? (*wa)[i] : glm::vec4(0.0f);
            vertices.push_back(v);
        }

        for(short i : *pi)
        {
            indicies.push_back(i);
        }

        delete pa; delete na; delete ta; delete ja; delete wa; delete pi;

        Texture *texture = nullptr;

        if(p.material != -1)
        {
            tinygltf::Material mat = model.materials[p.material];
            if(mat.pbrMetallicRoughness.baseColorTexture.index != -1)
                texture = myScene.textures[mat.pbrMetallicRoughness.baseColorTexture.index];
        }

        primitives.push_back(Primitive(vertices, indicies, texture));
    }

    Mesh *mesh = new Mesh(primitives);
    return mesh;
}

static std::vector<Animation *> loadAnimation(Scene &myScene, tinygltf::Model &model, tinygltf::Animation &a)
{
    std::vector<Animation *> anims;

    for(tinygltf::AnimationChannel &ac : a.channels)
    {
        Node *target = myScene.nodes[ac.target_node];
        tinygltf::AnimationSampler &as = a.samplers[ac.sampler];
        Accessor<float> *timeAccessor = getAccessor<float>(myScene, model, as.input);
        if(ac.target_path == "rotation")
        {
            Accessor<glm::quat> *dataAccessor = getAccessor<glm::quat>(myScene, model, as.output);
            anims.emplace_back(new QuatAnimation(dataAccessor, timeAccessor, *target));
        }
        else
        {
            Accessor<glm::vec3> *dataAccessor = getAccessor<glm::vec3>(myScene, model, as.output);

            if(ac.target_path == "translation")
            {
                anims.emplace_back(new Vec3Animation(dataAccessor, timeAccessor, *target, Vec3Animation::TRANSLATION));
            }
            else if(ac.target_path == "scale")
            {
                anims.emplace_back(new Vec3Animation(dataAccessor, timeAccessor, *target, Vec3Animation::SCALE));
            }
            else
            {
                LOGF("Unsupported animation path\n");
                throw std::runtime_error("Unsupported animation path\n");
            }
        }

    }

    return anims;
}

static Texture *loadTexture(tinygltf::Model &model, tinygltf::Texture &t)
{
    tinygltf::Image img = model.images[t.source];
    LOGD("Component: %d, Bits: %d, Pixel type: %d\n", img.component, img.bits, img.pixel_type);
    return new Texture(img.width, img.height, img.image);
}

template <typename T>
Accessor<T> *getAccessor(Scene &myScene, tinygltf::Model &model, int index)
{    
    if(index == -1)
        return nullptr;

    tinygltf::Accessor a = model.accessors[index];
    tinygltf::BufferView b = model.bufferViews[a.bufferView];

    BufferView bv = BufferView((long)b.byteOffset, (long)b.byteLength, (long)b.byteStride, myScene.buffers[b.buffer]);
    return new Accessor<T>(bv, (long)a.byteOffset, (long)a.count);
}
