#pragma once
#include <map>
#include <string>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

class Shader
{
public:
    Shader(const char* verStr, const char* fragStr);
    ~Shader();


private:
    GLuint LoadShader ( GLenum type, const char *shaderStr);
    void GetShaderUniforms(GLuint shaderProgram);

private:
    GLuint mShaderProgram;
    std::map<std::string, int> mAttributes;
};