#pragma once
#include <unordered_map>
#include <string>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "kazmath.h"


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

	void bind();
	void unbind();

	GLint	getUniformLocation( size_t hashedName );
	GLint	getAttribLocation( size_t hashedName );

	void	uniform( size_t name, int data );	
	void	uniform( size_t name, float data );
	void	uniform( size_t name, const kmVec2 &data );
	void	uniform( size_t name, const kmVec3 &data );
	void	uniform( size_t name, const kmVec4 &data );
	void	uniform( size_t name, const kmMat3 &data, bool transpose = false );
	void	uniform( size_t name, const kmMat4 &data, bool transpose = false );
	void	uniform( size_t name, const float *data, int count );
	void	uniform( size_t name, const kmVec2 *data, int count );
	void	uniform( size_t name, const kmVec3 *data, int count );
	void	uniform( size_t name, const kmVec4 *data, int count );
	void	uniform( size_t name, const kmMat3 *data, int count, bool transpose = false );
	void	uniform( size_t name, const kmMat4 *data, int count, bool transpose = false );

private:
    GLuint	loadShader ( GLenum type, const char *shaderStr);
    void	generateShaderInfo( GLuint shaderProgram);

private:
    GLuint				mShaderProgram;
	std::unordered_map<size_t, ShaderUniformDef*> mShaderUniformsInfo;
	std::unordered_map<size_t, ShaderAttributeDef*> mShaderAttributesInfo;
};


inline void Shader::bind()
{
	glUseProgram(mShaderProgram);
}

inline void Shader::unbind()
{
	//glUseProgram(0);
}

inline GLint Shader::getUniformLocation( size_t hashedName )
{
	auto val = mShaderUniformsInfo.find(hashedName);
	return (val != mShaderUniformsInfo.end() ? val->second->location : -1);
}

inline GLint Shader::getAttribLocation( size_t hashedName )
{
	auto val = mShaderAttributesInfo.find(hashedName);
	return (val != mShaderAttributesInfo.end() ? val->second->location : -1);
}

inline void	Shader::uniform( size_t name, int data )
{
	GLint loc = getUniformLocation(name);
	glUniform1i(loc, data);
}

inline void	Shader::uniform( size_t name, float data )
{
	GLint loc = getUniformLocation(name);
	glUniform1f(loc,data);
}

inline void	Shader::uniform( size_t name, const kmVec2 &data )
{
	GLint loc = getUniformLocation(name);
	glUniform2f(loc, data.x, data.y);
}

inline void	Shader::uniform( size_t name, const kmVec3 &data )
{
	GLint loc = getUniformLocation(name);
	glUniform3f(loc,data.x, data.y, data.z);
}

inline void	Shader::uniform( size_t name, const kmVec4 &data )
{
	GLint loc = getUniformLocation(name);
	glUniform4f(loc, data.x, data.y, data.z, data.w);
}

inline void	Shader::uniform( size_t name, const kmMat3 &data, bool transpose)
{
	GLint loc = getUniformLocation(name);
	glUniformMatrix3fv(loc,1,(transpose) ? GL_TRUE:GL_FALSE, data.mat);
}

inline void	Shader::uniform( size_t name, const kmMat4 &data, bool transpose)
{
	GLint loc = getUniformLocation(name);
	glUniformMatrix4fv(loc,1, (transpose) ? GL_TRUE : GL_FALSE, data.mat);
}

inline void	Shader::uniform( size_t name, const float *data, int count )
{
	GLint loc = getUniformLocation(name);
	glUniform1fv(loc, count, data);
}

inline void	Shader::uniform( size_t name, const kmVec2 *data, int count )
{
	GLint loc = getUniformLocation(name);
	glUniform2fv(loc, count, &data[0].x);
}

inline void	Shader::uniform( size_t name, const kmVec3 *data, int count )
{
	GLint loc = getUniformLocation(name);
	glUniform3fv(loc, count, &data[0].x);
}

inline void	Shader::uniform( size_t name, const kmVec4 *data, int count )
{
	GLint loc = getUniformLocation(name);
	glUniform4fv(loc, count, &data[0].x);
}

inline void	Shader::uniform( size_t name, const kmMat3 *data, int count, bool transpose)
{
	GLint loc = getUniformLocation(name);
	glUniformMatrix3fv(loc,count, (transpose) ? GL_TRUE : GL_FALSE, data->mat);
}

inline void	Shader::uniform( size_t name, const kmMat4 *data, int count, bool transpose)
{
	GLint loc = getUniformLocation(name);
	glUniformMatrix4fv( loc, count, ( transpose ) ? GL_TRUE : GL_FALSE, data->mat );
}