#include <stdexcept>

#include <GL/glew.h>

#include "application.hpp"

#include "scene/sceneloader.hpp"

#include "log.h"

Application::Application() 
{
    glfwSetErrorCallback(glfw_error_callback);

    if(!glfwInit())
        throw std::runtime_error("GLFW failed to initialize!");
    
    wnd = glfwCreateWindow(960, 540, ":3", NULL, NULL);
    if(!wnd)
        throw std::runtime_error("Failed to create window!");

    glfwMakeContextCurrent(wnd);

    GLenum err = glewInit();
    if(err != GLEW_OK)
        throw std::runtime_error("GLEW failed to initialize: " + std::string((char *) glewGetErrorString(err)));

    renderer.init();

    loadScene(this->scene, "./res/MyScene.gltf");
}

void Application::Run()
{
    for(Mesh *m : scene.meshes)
    {
        m->uploadToGPU();
    }


    while (!glfwWindowShouldClose(wnd))
    {
        renderer.drawScene(scene);
        glfwSwapBuffers(wnd);
        glfwPollEvents();
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
