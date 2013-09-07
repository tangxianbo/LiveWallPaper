#pragma once
#include <GLES2/gl2.h>
#include "shader.h"
#include "Mesh.h"

struct WaterVertex
{
	WaterVertex(){}
	WaterVertex(float x, float y, float z, float u, float v)
	{
		position[0] = x;
		position[1] = y;
		position[2] = z;
		uv[0] = u;
		uv[1] = v;
	}

	float position[3];
	float uv[2];
};

class Texture2D;
class FrameBuffer;
class Water
{
public:
	Water(int screenWidth, int screenHeight, float dx);
	~Water();

	void Init();
	void Render();

	void Touch(int x, int y);

private:
	void _initShader();
	void _initTexture();
	void _initMesh();

	void _drawQuad();

private:
	int				m_screenWidth;
	int				m_screenHeight;
	int				m_resWidth;
	int				m_resHeight;
	float			m_dx;
	int				m_numFaces;
	int				m_vertexBufferSize;
	int				m_indexEleNum;
	WaterVertex*	m_curVertexBuffer;
	WaterVertex*	m_preVertexBuffer;

	GLuint			m_backgroundTexture;

	MeshObject*		m_quadMesh;
	Shader*			m_quadShader;
	Shader*			m_dropShader;
	Shader*			m_updateShader;
	Shader*			m_normalRenderShader;

	Texture2D*		m_pingTexture;
	Texture2D*		m_pangTexture;

	FrameBuffer*	m_frameBuffer;
};