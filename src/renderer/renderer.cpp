#include "renderer/renderer.hpp"
#include <GL/glew.h>

void init()
{

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
