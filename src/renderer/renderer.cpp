#include "renderer/renderer.hpp"
#include <GL/glew.h>

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
		glm::mat4 modelMatrix = m->getNode()->getGlobalModelMatrix();
		shader.setModelMatrix(modelMatrix);

		m->draw();
	}
}

void renderScene()
{
	/*mat4 m4Transform = {0}, model = {0};
	GLuint *hProgram, hShaderTransform, hShaderMVP;

	hProgram = getShader("default");
	glUseProgram(*hProgram);
	hShaderTransform = glGetUniformLocation(*hProgram, "transform");
	hShaderMVP = glGetUniformLocation(*hProgram, "MVP");
	//LOGINFO("Program Handle:%d, Transform Handle: %d, MVP *handle: %d\n", *hProgram, hShaderTransform, hShaderMVP);

	for(int i = 0; i < activeEntities; i++)
	{
		glBindVertexArray(entities[i]->mesh.model->hVertexArrayObject);
		glBindTexture(GL_TEXTURE_2D, *player->hTexture);

		calculateMVP(camera, &entities[i]->mesh.transform, &m4Transform[0]);
		glUniformMatrix4fv(hShaderMVP, 1, GL_FALSE, (GLfloat*) &m4Transform);

		calculateModel(&entities[i]->mesh.transform, model);
		glUniformMatrix4fv(hShaderTransform, 1, GL_FALSE, (GLfloat*) model[0]);

		glDrawElements(GL_TRIANGLES, entities[i]->mesh.model->indices, GL_UNSIGNED_INT, NULL);
	}*/
}
