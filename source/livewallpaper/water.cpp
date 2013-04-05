#include "water.h"
#include "esutils.h"
#include <vector>

const int VERTEX_POS_INDX = 0;

#define REAL_WATER 1

Water::Water(int screenWidth, int screenHeight, float dx):	m_screenWidth(screenWidth)
															,m_screenHeight(screenHeight)
															,m_resWidth(0)
															,m_resHeight(0)
															,m_dx(dx)
															,m_vertexBufferSize(0)
															,m_indexEleNum(0)
															,m_curVertexBuffer(nullptr)
															,m_preVertexBuffer(nullptr)
															,m_programObject(NULL)
															,m_vertexBuffer(NULL)
															,m_indexBuffer(NULL)
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
	glDrawElements(GL_TRIANGLES,m_numFaces*3,GL_UNSIGNED_SHORT,NULL);

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
#if 1
	m_resWidth = int(m_screenWidth/m_dx+0.5f) + 1;
	m_resHeight = int(m_screenHeight/m_dx+0.5f) + 1;
	m_vertexBufferSize = sizeof(WaterVertex)*m_resWidth*m_resHeight;

	m_numFaces = (m_resWidth-1)*(m_resHeight-1)*2;

	m_curVertexBuffer = new WaterVertex[m_resWidth*m_resHeight];
	m_preVertexBuffer = new WaterVertex[m_resWidth*m_resHeight];

	float halfWidth = m_screenWidth*0.5f;
	float halfHeight = m_screenHeight*0.5f;

	for (int i = 0; i < m_resHeight; i++)
	{
		float z = halfHeight - i*m_dx;
		for (int j=0; j< m_resWidth; j++)
		{
			float x = -halfWidth + j*m_dx;

#if 1
			m_curVertexBuffer[i*m_resWidth + j] = WaterVertex(j*2.0f/(m_resWidth-1)-1.0f,1.0f-i*2.0f/(m_resHeight-1),0.5f);
			m_preVertexBuffer[i*m_resWidth + j] = WaterVertex(j*2.0f/(m_resWidth-1)-1.0f,1.0f-i*2.0f/(m_resHeight-1),0.5f);
#else
			m_curVertexBuffer[i*m_resWidth + j] = WaterVertex(x,0.0f,z);
			m_preVertexBuffer[i*m_resWidth + j] = WaterVertex(x,0.0f,z);
#endif
		}
	}


	std::vector<GLushort> indices(m_numFaces*3);
	int k = 0;
	for (int i=0; i<m_resHeight-1; ++i)
	{
		for (int j=0; j<m_resWidth-1; ++j)
		{
			indices[k++] = i*m_resWidth+j;
			indices[k++] = i*m_resWidth+j+1;
			indices[k++] = (i+1)*m_resWidth+j;

			indices[k++] = (i+1)*m_resWidth+j;
			indices[k++] = i*m_resWidth+j+1;
			indices[k++] = (i+1)*m_resWidth+j+1;
		}
	}

	glGenBuffers(1,&m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,m_vertexBufferSize, m_curVertexBuffer,GL_STATIC_DRAW);

	glGenBuffers(1,&m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLushort)*m_numFaces*3,&indices[0],GL_STATIC_DRAW);

#else
	m_numFaces = 1;
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
#endif
}