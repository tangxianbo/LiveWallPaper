#include "Renderer.h"
#include "Mesh.h"
#include "shader.h"

namespace Renderer
{


void RenderMesh(const MeshObject* mesh, const Shader* shader)
{
	GLuint vbo = mesh->getVBO();
	GLuint ibo = mesh->getIBO();

	glBindBuffer(GL_ARRAY_BUFFER,vbo);

	for (auto it = shader->getVertexAttributesBegin(); 
		it != shader->getVertexAttributesEnd(); 
		++it)
	{

		E_Vertex_Attribute attribute = it->attributeType;
		s32  attributesLoc = it->location;
		const MeshAttributeDef* meshAttribute = mesh->getMeshAttribute(attribute);

		glVertexAttribPointer(attributesLoc,
			meshAttribute->m_elementNum,
			meshAttribute->m_elementType,
			0,
			meshAttribute->m_stride, 
			reinterpret_cast<u32*>(meshAttribute->m_offset)
			);

		glEnableVertexAttribArray(attributesLoc);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
	glDrawElements(GL_TRIANGLES, 
		mesh->getIndexCount(),
		GL_UNSIGNED_SHORT,
		NULL);

	for (auto it = shader->getVertexAttributesBegin(); 
		it != shader->getVertexAttributesEnd(); 
		++it)
	{
		glDisableVertexAttribArray(it->location);
	}
}

}