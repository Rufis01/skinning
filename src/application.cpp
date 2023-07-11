#include <stdexcept>

#include <GL/glew.h>

#include "application.hpp"

#include "renderer/scene/sceneloader.hpp"
#include "renderer/scene/mesh/primitive.hpp"

#include "log.h"

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  printf( "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

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

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);  
    glDebugMessageCallback(MessageCallback, 0);

    loadScene(this->scene, "./res/Suzanne.gltf");
    
}

void Application::Run()
{
    LOGD("Size of Vertex: %d\n", sizeof(Vertex));
    for(Mesh *m : scene.meshes)
    {
        m->uploadToGPU();
    }

    Shader s;
    s.loadFragmentShaderFromFile("./res/base.fsh");
    s.loadVertexShaderFromFile("./res/base.vsh");

    s.bind();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(wnd))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
        for(Mesh *m : scene.meshes)
        {
            m->draw();
        }
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
