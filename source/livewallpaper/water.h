#pragma once
#include <GLES2/gl2.h>

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

	//object and buffer
	GLuint			m_shader_waterdisplay;
	GLuint			m_shader_drop;
	GLuint			m_vertexBuffer;
	GLuint			m_indexBuffer;
	GLuint			m_textureObject;

	//attribute and uniforms location
	GLint			m_positionIndex;
	GLint			m_uvIndex;
	GLint			m_heightMapIndex;
	GLint			m_textureIndex;

	Texture2D*		m_pingTexture;
	Texture2D*		m_pangTexture;
	FrameBuffer*	m_frameBuffer;
};