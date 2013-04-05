#pragma once
#include <GLES2/gl2.h>

class Water
{
public:
	Water();
	~Water();

	void Init();
	void Render();

private:
	void _initShader();
	void _initTexture();
	void _initMesh();

private:
	GLuint		m_programObject;
	GLuint		m_vertexBuffer;
	GLuint		m_indexBuffer;
};