#pragma once
#include <GLES3/gl3.h>
#include <unordered_map>
#include "base/Types.h"
#include "EVertexAttribute.h"

struct MeshAttributeDef
{
	GLint		m_elementNum;
	GLenum		m_elementType;
	GLsizei		m_stride;
	u32 		m_offset;
};

class MeshObject
{
public:
	MeshObject(GLuint vbo, GLuint ibo);
    MeshObject();
	~MeshObject();

	void setVBO(GLuint vbo);
    void setIBO(GLuint ibo);

	GLuint getVBO() const;
	GLuint getIBO() const;

    void setIndexCount(u32 count);
    u32 getIndexCount() const;

    void addMeshAttribute(const char* attributeName, 
                            GLint elementNum, 
                            GLenum elementType, 
                            GLsizei stride, 
                            u32 offset);
	const MeshAttributeDef* getMeshAttribute(E_Vertex_Attribute attribute) const;

private:
    typedef std::unordered_map<E_Vertex_Attribute,MeshAttributeDef> AttributeMap;
    std::unordered_map<E_Vertex_Attribute,MeshAttributeDef> m_attributeMap;

	GLuint m_VBO;
	GLuint m_IBO;

    u32 m_indexCount;
};


inline void 
MeshObject::setVBO(GLuint vbo)
{
    this->m_VBO = vbo;
}

inline GLuint 
MeshObject::getVBO() const
{
    return m_VBO;
}

inline GLuint 
MeshObject::getIBO() const
{
    return m_IBO;
}

inline void 
MeshObject::setIBO(GLuint ibo)
{
    this->m_IBO = ibo;
}

inline void 
MeshObject::setIndexCount(u32 count)
{
    m_indexCount = count;
}

inline u32
MeshObject::getIndexCount() const
{
    return m_indexCount;
}
