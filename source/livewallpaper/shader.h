#pragma once
#include <unordered_map>
#include <string>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <core/types.h>
#include <math/vector2d.h>
#include <math/vector3d.h>
#include <math/vector4d.h>
#include <math/matrix4.h>
#include "EVertexAttribute.h"

using namespace jenny;

struct ShaderUniformDef
{
	int		location;
	int		arraySize;
	GLenum	valueType;
};

struct ShaderAttributeDef
{
    E_Vertex_Attribute  attributeType;
	s32                 location;
};

class Shader
{
public:
    Shader(const char* verStr, const char* fragStr);
    ~Shader();

	void bind() const;
	void unbind() const;

	GLint	getUniformLocation( size_t hashedName );

    typedef const ShaderAttributeDef* VertexAttributeIter;

    VertexAttributeIter getVertexAttributesBegin() const;
    VertexAttributeIter getVertexAttributesEnd() const;

	void	uniform( size_t name, int data );	
	void	uniform( size_t name, float data );
	void	uniform( size_t name, const vector2df &data );
	void	uniform( size_t name, const vector3df &data );
	void	uniform( size_t name, const vector4df &data );
	void	uniform( size_t name, const matrix3 &data, bool transpose = false );
	void	uniform( size_t name, const matrix4 &data, bool transpose = false );
	void	uniform( size_t name, const float *data, int count );
	void	uniform( size_t name, const vector2df *data, int count );
	void	uniform( size_t name, const vector3df *data, int count );
	void	uniform( size_t name, const vector4df *data, int count );
	void	uniform( size_t name, const matrix3 *data, int count, bool transpose = false );
	void	uniform( size_t name, const matrix4 *data, int count, bool transpose = false );

private:
    GLuint	loadShader ( GLenum type, const char *shaderStr);
    void	generateShaderInfo( GLuint shaderProgram);

private:
    GLuint				    mShaderProgram;
    ShaderAttributeDef*     m_ShaderAttributes;
    u32                     m_ShaderAttributesNum;
	std::unordered_map<size_t, ShaderUniformDef*> mShaderUniformsInfo;
};


inline void 
Shader::bind() const
{
	glUseProgram(mShaderProgram);
}

inline void 
Shader::unbind() const
{
	//glUseProgram(0);
}

inline GLint 
Shader::getUniformLocation( size_t hashedName )
{
	auto val = mShaderUniformsInfo.find(hashedName);
	return (val != mShaderUniformsInfo.end() ? val->second->location : -1);
}


inline Shader::VertexAttributeIter 
Shader::getVertexAttributesBegin() const
{
    return m_ShaderAttributes;
}


inline Shader::VertexAttributeIter 
Shader::getVertexAttributesEnd() const
{
    return (m_ShaderAttributes + m_ShaderAttributesNum);
}


inline void	
Shader::uniform( size_t name, int data )
{
	GLint loc = getUniformLocation(name);
	glUniform1i(loc, data);
}


inline void	
Shader::uniform( size_t name, float data )
{
	GLint loc = getUniformLocation(name);
	glUniform1f(loc,data);
}

inline void	
Shader::uniform( size_t name, const vector2df &data )
{
	GLint loc = getUniformLocation(name);
	glUniform2f(loc, data.x(), data.y());
}

inline void	
Shader::uniform( size_t name, const vector3df &data )
{
	GLint loc = getUniformLocation(name);
	glUniform3f(loc,data.x(), data.y(), data.z());
}

inline void	
Shader::uniform( size_t name, const vector4df &data )
{
	GLint loc = getUniformLocation(name);
	glUniform4f(loc, data.x(), data.y(), data.z(), data.w());
}

inline void	
Shader::uniform( size_t name, const matrix3 &data, bool transpose)
{
	GLint loc = getUniformLocation(name);
	glUniformMatrix3fv(loc,1,(transpose) ? GL_TRUE:GL_FALSE, data.pointer());
}

inline void	
Shader::uniform( size_t name, const matrix4 &data, bool transpose)
{
	GLint loc = getUniformLocation(name);
	glUniformMatrix4fv(loc,1, (transpose) ? GL_TRUE : GL_FALSE, data.pointer());
}

inline void	
Shader::uniform( size_t name, const float *data, int count )
{
	GLint loc = getUniformLocation(name);
	glUniform1fv(loc, count, data);
}

inline void	
Shader::uniform( size_t name, const vector2df *data, int count )
{
	GLint loc = getUniformLocation(name);
	glUniform2fv(loc, count, data->getDataPtr());
}

inline void	
Shader::uniform( size_t name, const vector3df *data, int count )
{
	GLint loc = getUniformLocation(name);
	glUniform3fv(loc, count, data->getDataPtr());
}

inline void	
Shader::uniform( size_t name, const vector4df *data, int count )
{
	GLint loc = getUniformLocation(name);
	glUniform4fv(loc, count, data->getDataPtr());
}

inline void	
Shader::uniform( size_t name, const matrix3 *data, int count, bool transpose)
{
	GLint loc = getUniformLocation(name);
	glUniformMatrix3fv(loc,count, (transpose) ? GL_TRUE : GL_FALSE, data->pointer());
}

inline void	
Shader::uniform( size_t name, const matrix4 *data, int count, bool transpose)
{
	GLint loc = getUniformLocation(name);
	glUniformMatrix4fv( loc, count, ( transpose ) ? GL_TRUE : GL_FALSE, data->pointer());
}