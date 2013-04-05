#include "water.h"
#include "esutils.h"

const int VERTEX_POS_INDX = 0;

Water::Water()
{

}

Water::~Water()
{

}

void Water::Init()
{
	this->_initShader();
	this->_initTexture();
	this->_initMesh();
}

void Water::Render()
{
	glUseProgram (m_programObject);

	glBindBuffer(GL_ARRAY_BUFFER,m_vertexBuffer);
	glVertexAttribPointer(VERTEX_POS_INDX,3,GL_FLOAT,0,0,NULL);
	glEnableVertexAttribArray(VERTEX_POS_INDX);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_indexBuffer);
	glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_SHORT,NULL);

	glDisableVertexAttribArray(VERTEX_POS_INDX);
}

void Water::_initShader()
{
	const char vShaderStr[] =  
		"attribute vec4 vPosition;    \n"
		"void main()                  \n"
		"{                            \n"
		"   gl_Position = vPosition;  \n"
		"}                            \n";

	const char fShaderStr[] =  
		"precision mediump float;\n"\
		"void main()                                  \n"
		"{                                            \n"
		"  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
		"}                                            \n";

	GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
	GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER,fShaderStr);

	m_programObject = glCreateProgram();
	if(0 == m_programObject)
		return;

	glAttachShader(m_programObject,vertexShader);
	glAttachShader(m_programObject,fragmentShader);

	glBindAttribLocation(m_programObject,VERTEX_POS_INDX,"vPosition");

	glLinkProgram(m_programObject);

	GLint linked;
	glGetProgramiv(m_programObject,GL_LINK_STATUS,&linked);
	if(!linked)
	{
		glDeleteProgram(m_programObject);
		m_programObject = 0;
		return;
	}
}

void Water::_initTexture()
{

}


void Water::_initMesh()
{
	GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f, 
							-0.5f, -0.5f, 0.0f,
							0.5f, -0.5f, 0.0f };
	glGenBuffers(1,&m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vVertices),vVertices,GL_STATIC_DRAW);

	GLushort vIndex[] = {0,1,2};
	glGenBuffers(1,&m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(vIndex),vIndex,GL_STATIC_DRAW);
}