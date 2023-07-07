#include <stdexcept>

#include <GL/glew.h>

#include "application.hpp"

#include "renderer/scene/sceneloader.hpp"

#include "log.h"


Application::Application() 
{
    renderer = Renderer();

    glfwSetErrorCallback(glfw_error_callback);

    if(!glfwInit())
        throw std::runtime_error("GLFW failed to initialize!");
    
    wnd = glfwCreateWindow(800, 600, ":3", NULL, NULL);
    if(!wnd)
        throw std::runtime_error("Failed to create window!");

    glfwMakeContextCurrent(wnd);

    GLenum err = glewInit();
    if(err != GLEW_OK)
        throw std::runtime_error("GLEW failed to initialize: " + std::string((char *) glewGetErrorString(err)));

    Scene s;
    loadScene(s, "./res/Triangle.gltf");
}

void Application::Run()
{
    while (!glfwWindowShouldClose(wnd))
    {
        glfwWaitEvents();
    }
}

Application::~Application()
{
    glfwDestroyWindow(wnd);
    glfwTerminate();
    LOGD("Application destroyed.\n");
}

void Application::glfw_error_callback(int error, const char* description)
{
    LOGE("GLFW: %s\n", description);
}
