#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "scene/scene.hpp"
#include "renderer/shader.hpp"

class Renderer {
    public:
    Renderer();
    void init();
    void drawScene(Scene &scene);
    private:
    Shader shader;
};

#endif
