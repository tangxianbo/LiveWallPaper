#include "water.h"
#include "esutils.h"
#include <vector>
#include <ktx.h>


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
	,m_textureObject(NULL)
	,m_positionIndex(-1)
	,m_uvIndex(-1)
	,m_heightMapIndex(-1)
	,m_textureIndex(-1)
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
	glUseProgram(m_programObject);

	glBindBuffer(GL_ARRAY_BUFFER,m_vertexBuffer);
	glVertexAttribPointer(m_positionIndex,3,GL_FLOAT,0,sizeof(WaterVertex),NULL);
	float* uvOffset = reinterpret_cast<float*>(12);
	glVertexAttribPointer(m_uvIndex,2,GL_FLOAT,0,sizeof(WaterVertex),uvOffset);

	glEnableVertexAttribArray(m_positionIndex);
	glEnableVertexAttribArray(m_uvIndex);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_textureObject);
	//glUniform1i(m_textureIndex, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_indexBuffer);
	glDrawElements(GL_TRIANGLES,m_numFaces*3,GL_UNSIGNED_SHORT,NULL);

	glDisableVertexAttribArray(m_positionIndex);
	glDisableVertexAttribArray(m_uvIndex);
}

void Water::_initShader()
{
	const char vShaderStr[] =  
		"attribute vec4 vPosition;		\n"
		"attribute vec2 vTexCoord;		\n"
		"varying vec2 v_texCoord;		\n"
		"void main()					\n"
		"{								\n"
		"   gl_Position = vPosition;	\n"
		"   v_texCoord = vTexCoord;		\n"	
		"}								\n";

	const char fShaderStr[] =  
		"precision mediump float;								\n"
		"varying vec2 v_texCoord;								\n"
		"uniform sampler2D s_texture;							\n"
		"void main()											\n"
		"{														\n"
		"  gl_FragColor = texture2D( s_texture, v_texCoord);	\n"
		"}														\n";

	m_programObject = esLoadProgram(vShaderStr,fShaderStr);
	if(0 == m_programObject)
		return;

	m_positionIndex = glGetAttribLocation(m_programObject,"vPosition");
	m_uvIndex = glGetAttribLocation(m_programObject,"vTexCoord");

	m_textureIndex = glGetUniformLocation(m_programObject,"s_texture");
}

void Water::_initTexture()
{
	GLenum		target;
	GLenum		glerror;
	GLboolean	isMipmapped;
	KTX_error_code ktxerror;

	FILE* ktxFile = fopen("triangle.ktx","rb");
	if (ktxFile)
	{
		fseek(ktxFile,0L,SEEK_END);
		long fileSize = ftell(ktxFile);
		char* fileData = new char[fileSize];
		fseek(ktxFile,0L,SEEK_SET);
		long readSize = fread(fileData,sizeof(char),fileSize,ktxFile);

		ktxerror = ktxLoadTextureM(fileData,fileSize,&m_textureObject,&target,NULL,&isMipmapped,&glerror,0,NULL);
		if (KTX_SUCCESS == ktxerror)
		{
			if (isMipmapped) 
				glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			else
				glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
	}

}


void Water::_initMesh()
{
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
			float u = j*1.0f/(m_resWidth-1);
			float v = i*1.0f/(m_resHeight-1);
			m_curVertexBuffer[i*m_resWidth + j] = WaterVertex(j*2.0f/(m_resWidth-1)-1.0f,1.0f-i*2.0f/(m_resHeight-1),0.5f,u,v);
			m_preVertexBuffer[i*m_resWidth + j] = WaterVertex(j*2.0f/(m_resWidth-1)-1.0f,1.0f-i*2.0f/(m_resHeight-1),0.5f,u,v);
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
}