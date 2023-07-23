#include "renderer/renderer.hpp"
#include <GL/glew.h>
#include <iostream>

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>

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
	if(type == GL_DEBUG_TYPE_ERROR)
		LOGE("GL CALLBACK: type = 0x%x, severity = 0x%x, message = %s\n", type, severity, message);
  	else
  		LOGD("GL CALLBACK: type = 0x%x, severity = 0x%x, message = %s\n", type, severity, message);
}

Renderer::Renderer()
{
}

void Renderer::init()
{
    glViewport(0, 0, 960, 540);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);  
    glDebugMessageCallback(MessageCallback, 0);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    shader.loadFragmentShaderFromFile("./res/base.fsh");
    shader.loadVertexShaderFromFile("./res/base.vsh");

    shader.bind();
}

void Renderer::drawScene(Scene &scene)
{
	Camera *activeCamera = *scene.cameras.begin();
	shader.setViewProjectionMatrix(activeCamera->getVPMatrix());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
	for(Mesh *m : scene.meshes)
	{
		if(m->getNode()->skin)
			m->getNode()->skin->bind();

		m->draw();
	}
}
