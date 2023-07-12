#ifndef APPLICATION_HPP
#define APPLICATION_HPP


#include "scene/scene.hpp"
#include "renderer/renderer.hpp"

#include <GLFW/glfw3.h>

class Application
{
    public:
    Application();
    ~Application();

    void Run();

    private:
    Renderer renderer;
    Scene scene;
    GLFWwindow *wnd;
    static void glfw_error_callback(int error, const char* description);
};

#endif
