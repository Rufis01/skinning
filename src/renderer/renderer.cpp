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
		glm::mat4 modelMatrix = m->getNode()->getGlobalModelMatrix();
/*
        glm::vec3 scale;
		glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;

        glm::decompose(modelMatrix, scale, rotation, translation, skew, perspective);

		glm::vec3 euler = glm::eulerAngles(rotation) * 180.0f / 3.14159f;
		LOGD("Node %s has rotation X:%f Y:%f Z:%f\n",  m->getNode()->name.c_str(), euler.x, euler.y, euler.z);
*/
		shader.setModelMatrix(modelMatrix);

		m->draw();
	}
//	getchar();
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
