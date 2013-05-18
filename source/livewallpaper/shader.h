#pragma once
#include <unordered_map>
#include <string>
#include <GLES2/gl2.h>
#include <EGL/egl.h>


struct ShaderUniformDef
{
	size_t  hashedName;
	int		location;
	int		arraySize;
	GLenum	valueType;
};

class Shader
{
public:
    Shader(const char* verStr, const char* fragStr);
    ~Shader();


private:
    GLuint LoadShader ( GLenum type, const char *shaderStr);
    void GetShaderUniforms(GLuint shaderProgram);

private:
    GLuint				mShaderProgram;
    ShaderUniformDef*	mShaderUniforms;
};