#pragma once
#include <GLES3/gl3.h>
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

	void Update();
	void Render();

	void onTouch(int x, int y);

private:
	void _initShader();
	void _initTexture();
	void _initMesh();

	void _drawQuad();
    void _renderMesh(const MeshObject* mesh, const Shader* shader);

	void _processTouch(int x, int y);

	void _doUpdate();
	void _updateNormal();
	void _genCaustics();

private:
	int				m_screenWidth;
	int				m_screenHeight;
	int				m_indexEleNum;
	float			m_screenScaleX;

	//object and buffer
	GLuint			m_shader_waterdisplay;
	//GLuint			m_shader_drop;
	GLuint			m_vertexBuffer;
	GLuint			m_indexBuffer;
	GLuint			m_textureObject;

	GLuint			m_quadVertexBuffer;
	GLuint			m_quadIndexBuffer;
	MeshObject*		m_screenRect;
	MeshObject*		m_waterMesh;
	MeshObject*		m_testTriangle;


	//attribute and uniforms location
	GLint			m_positionIndex;
	GLint			m_uvIndex;
	GLint			m_heightMapIndex;
	GLint			m_textureIndex;

	Texture2D*		m_pingTexture;
	Texture2D*		m_pangTexture;
	FrameBuffer*	m_frameBufferA;
	FrameBuffer*	m_frameBufferB;
	FrameBuffer*	m_frameBufferCaustic;

	Shader*			m_shader;
	Shader*			m_quadShader;
	Shader*			m_shader_drop;
	Shader*			m_shader_update;
	Shader*			m_shader_normal;
	Shader*			m_shader_water;
	Shader*			m_shader_caustics;
};

inline void Water::onTouch(int x, int y)
{
	this->_processTouch(x,y);
}