#pragma once
#include <GLES2/gl2.h>

struct WaterVertex
{
	WaterVertex(){}
	WaterVertex(float x, float y, float z)
	{
		position[0] = x;
		position[1] = y;
		position[2] = z;
	}

	float position[3];
};

class Water
{
public:
	Water(int screenWidth, int screenHeight, float dx);
	~Water();

	void Init();
	void Render();

private:
	void _initShader();
	void _initTexture();
	void _initMesh();

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
	GLuint			m_programObject;
	GLuint			m_vertexBuffer;
	GLuint			m_indexBuffer;
};