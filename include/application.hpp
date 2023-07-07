#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <GLFW/glfw3.h>

#include "renderer/scene/scene.hpp"
#include "renderer/shader.hpp"
#include "renderer/renderer.hpp"

class Application
{
    public:
    Application();
    ~Application();

    void Run();

    private:
    Renderer renderer;
    Shader shader;
    Scene scene;
    GLFWwindow *wnd;
    static void glfw_error_callback(int error, const char* description);
};

#endif
