#pragma once
#include <GLES2/gl2.h>
#include "base/Types.h"

struct MeshAttributeDef
{
	size_t		m_attributeName;
	GLint		m_elementNum;
	GLenum		m_elementType;
	GLsizei		m_stride;
	u32*		m_offsetPtr;
};

class MeshObject
{
public:
	MeshObject(GLuint vbo, GLuint ibo, int attributeSize):m_VBO(vbo),m_IBO(ibo)
	{
		
	}

	MeshObject():m_VBO(0),m_IBO(0)
	{

	}

	~MeshObject()
	{
		glDeleteBuffers(1,&m_VBO);
		glDeleteBuffers(1,&m_IBO);
	}


	void setVBO(GLuint vbo)
	{
		this->m_VBO = vbo;
	}

	GLuint getVBO()
	{
		return m_VBO;
	}

	GLuint getIBO()
	{
		return m_IBO;
	}

	void setIBO(GLuint ibo)
	{
		this->m_IBO = ibo;
	}

	MeshAttributeDef* getMeshAttribute(size_t hashedName)
	{
		for ()
		{
		}
	}

private:
	GLuint m_VBO;
	GLuint m_IBO;
};
