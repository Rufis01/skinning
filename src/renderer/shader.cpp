#include "renderer/shader.hpp"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>

#include "log.h"

Shader::Shader() = default;

void Shader::loadVertexShaderFromFile(std::string path)
{
    this->vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
    loadShaderFromFile(path, this->vertexShaderHandle);
    compileShader(this->vertexShaderHandle);
}

void Shader::loadFragmentShaderFromFile(std::string path)
{
    this->fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
    loadShaderFromFile(path, this->fragmentShaderHandle);
    compileShader(this->fragmentShaderHandle);
}

void Shader::loadShaderFromFile(std::string path, GLuint handle)
{
    std::ifstream stream;
    stream.open(path);
    std::string str(std::istreambuf_iterator<char>{stream}, {});
    const char *text = str.c_str();

    LOGD("%s\n", text);

    glShaderSource(handle, 1, &text, nullptr);
}

void Shader::compileShader(GLuint handle)
{
    GLint glStatus = 0;
    glCompileShader(handle);
    glGetShaderiv(handle, GL_COMPILE_STATUS, &glStatus);

    if(glStatus != GL_TRUE)
    {
        int errorLength = 0;
        char *glError = nullptr;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &errorLength);

        if(errorLength != 0)
            glError = (char *) malloc(errorLength);

        if(glError != nullptr)
        {
            glGetShaderInfoLog(handle, errorLength, nullptr, glError);
            LOGE("Shader Compilation failed! %s\n", glError);
            free(glError);
        }
        else
        {
            LOGE("Shader Compilation failed due to an unknown error!\n");
        }
    }
}

void Shader::linkAndValidate()
{
    this->programHandle = glCreateProgram();

    glBindAttribLocation(this->programHandle, 0, "position");
    glBindAttribLocation(this->programHandle, 1, "normal");
    glBindAttribLocation(this->programHandle, 2, "uv");
    glBindAttribLocation(this->programHandle, 3, "joints");
    glBindAttribLocation(this->programHandle, 4, "weights");

    glAttachShader(this->programHandle, this->vertexShaderHandle);
    glAttachShader(this->programHandle, this->fragmentShaderHandle);

    glLinkProgram(this->programHandle);

    glValidateProgram(this->programHandle);

    glDeleteShader(this->vertexShaderHandle);
    glDeleteShader(this->fragmentShaderHandle);

    this->modelMatrixHandle = glGetUniformLocation(this->programHandle, "M");
    this->VPMatrixHandle    = glGetUniformLocation(this->programHandle, "VP");
}

void Shader::bind()
{
    if(this->programHandle == -1)
        linkAndValidate();

    glUseProgram(this->programHandle);
}

void Shader::setModelMatrix(glm::mat4 &M)
{
    glUniformMatrix4fv(modelMatrixHandle, 1, GL_FALSE, (const float*)glm::value_ptr(M));
}

void Shader::setViewProjectionMatrix(glm::mat4 &VP)
{
    glUniformMatrix4fv(VPMatrixHandle, 1, GL_FALSE, (const float*)glm::value_ptr(VP));
}
