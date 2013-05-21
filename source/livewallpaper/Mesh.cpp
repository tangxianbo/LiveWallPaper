#include "Mesh.h"

MeshObject::MeshObject(GLuint vbo, GLuint ibo):m_VBO(vbo)
                                              ,m_IBO(ibo)
                                              ,m_indexCount(0)
{
}

MeshObject::MeshObject():m_VBO(0),m_IBO(0)
{
}

MeshObject::~MeshObject()
{
    glDeleteBuffers(1,&m_VBO);
    glDeleteBuffers(1,&m_IBO);
}

void MeshObject::addMeshAttribute(const char* attributeName, 
                                  GLint elementNum, 
                                  GLenum elementType, 
                                  GLsizei stride, 
                                  u32 offset)
{
    E_Vertex_Attribute attributeType = getShaderVertexAttribute(attributeName);

    MeshAttributeDef attributeDef;
    attributeDef.m_elementNum = elementNum;
    attributeDef.m_elementType = elementType;
    attributeDef.m_stride = stride;
    attributeDef.m_offset = offset;

    m_attributeMap.insert(std::pair<E_Vertex_Attribute, MeshAttributeDef>(attributeType,attributeDef));
}

const MeshAttributeDef* MeshObject::getMeshAttribute(E_Vertex_Attribute attribute) const
{
    AttributeMap::const_iterator it = m_attributeMap.find(attribute);
    if (it == m_attributeMap.end())
    {
        return nullptr;
    }
    return &it->second;
}