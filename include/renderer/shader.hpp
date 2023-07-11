#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <GL/glew.h>

class Shader
{
    public:
    Shader();

    void loadVertexShaderFromFile(std::string path);
    void loadFragmentShaderFromFile(std::string path);
    void bind();

    private:

    GLuint fragmentShaderHandle = -1;
    GLuint vertexShaderHandle = -1;

    GLuint programHandle = -1;

    void loadShaderFromFile(std::string path, GLuint handle);
    void compileShader(GLuint handle);
    void linkAndValidate();
};

#endif