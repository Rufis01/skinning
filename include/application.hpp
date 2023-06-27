#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <GLFW/glfw3.h>

#include "scene.hpp"
#include "shader.hpp"
#include "renderer.hpp"

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
