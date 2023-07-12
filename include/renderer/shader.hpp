#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader
{
    public:
    Shader();

    void loadVertexShaderFromFile(std::string path);
    void loadFragmentShaderFromFile(std::string path);
    void setModelMatrix(glm::mat4 &M);
    void setViewProjectionMatrix(glm::mat4 &VP);
    void bind();

    private:

    GLuint fragmentShaderHandle = -1;
    GLuint vertexShaderHandle = -1;

    GLuint programHandle = -1;

    GLint modelMatrixHandle = -1;
    GLint VPMatrixHandle = -1;

    void loadShaderFromFile(std::string path, GLuint handle);
    void compileShader(GLuint handle);
    void linkAndValidate();
};

#endif