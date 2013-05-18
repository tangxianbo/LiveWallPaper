#pragma once
#include <unordered_map>
#include <string>
#include <GLES2/gl2.h>
#include <EGL/egl.h>


struct ShaderUniformDef
{
	int		location;
	int		arraySize;
	GLenum	valueType;
};

struct ShaderAttributeDef
{
	int    location;
};

class Shader
{
public:
    Shader(const char* verStr, const char* fragStr);
    ~Shader();

	void Bind();

private:
    GLuint LoadShader ( GLenum type, const char *shaderStr);
    void GenerateShaderInfos(GLuint shaderProgram);

private:
    GLuint				mShaderProgram;
	std::unordered_map<size_t, ShaderUniformDef*> mShaderUniformsInfo;
	std::unordered_map<size_t, ShaderAttributeDef*> mShaderAttributesInfo;
};


inline void Shader::Bind()
{
	glUseProgram(mShaderProgram);
}