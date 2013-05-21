#include "water.h"
#include "esutils.h"
#include <vector>
#include <ktx.h>
#include "texture2d.h"
#include "framebuffer.h"
#include <string>
#include "shader.h"
#include "base/string_hash.h"
#include "kazmath.h"
#include "esutils.h"


Water::Water(int screenWidth, int screenHeight, float dx):	m_screenWidth(screenWidth)
	,m_screenHeight(screenHeight)
	,m_resWidth(0)
	,m_resHeight(0)
	,m_dx(dx)
	,m_vertexBufferSize(0)
	,m_indexEleNum(0)
	,m_curVertexBuffer(nullptr)
	,m_preVertexBuffer(nullptr)
	,m_shader_waterdisplay(NULL)
	,m_shader_drop(NULL)
	,m_vertexBuffer(NULL)
	,m_indexBuffer(NULL)
	,m_textureObject(NULL)
	,m_positionIndex(-1)
	,m_uvIndex(-1)
	,m_heightMapIndex(-1)
	,m_textureIndex(-1)
	,m_pingTexture(nullptr)
	,m_pangTexture(nullptr)
	,m_frameBuffer(0)
{
}

Water::~Water()
{

}

void Water::Init()
{
	const GLubyte* extension = glGetString(GL_EXTENSIONS);

	this->_initShader();
	this->_initMesh();
	this->_initTexture();

	//m_frameBuffer = new FrameBuffer(m_resWidth,m_resHeight);
}

void Water::Render()
{


	//render to buffer benign.
	//{
	//	m_frameBuffer->Begin();
	//	glUseProgram(m_shader_waterdisplay);
	//	glBindBuffer(GL_ARRAY_BUFFER,m_vertexBuffer);
	//	glVertexAttribPointer(m_positionIndex,3,GL_FLOAT,0,sizeof(WaterVertex),NULL);
	//	float* uvOffset = reinterpret_cast<float*>(12);
	//	glVertexAttribPointer(m_uvIndex,2,GL_FLOAT,0,sizeof(WaterVertex),uvOffset);

	//	glEnableVertexAttribArray(m_positionIndex);
	//	glEnableVertexAttribArray(m_uvIndex);

	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D,m_textureObject);

	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_indexBuffer);
	//	glDrawElements(GL_TRIANGLES,m_numFaces*3,GL_UNSIGNED_SHORT,NULL);
	//	m_frameBuffer->End();
	//}
	//render to buffer end.

#if 0
	glViewport(0,0,m_screenWidth,m_screenHeight);

	m_shader->bind();
		kmVec2 offset; offset.x = 0.1f; offset.y = 0.0f;
		m_shader->uniform(CTHASH("v_offset"), offset);

		glBindBuffer(GL_ARRAY_BUFFER,m_vertexBuffer);
		glVertexAttribPointer(m_positionIndex,3,GL_FLOAT,0,sizeof(WaterVertex),NULL);
		float* uvOffset = reinterpret_cast<float*>(12);
		glVertexAttribPointer(m_uvIndex,2,GL_FLOAT,0,sizeof(WaterVertex),uvOffset);

		glEnableVertexAttribArray(m_positionIndex);
		glEnableVertexAttribArray(m_uvIndex);

		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D,m_pingTexture->getId());
		glBindTexture(GL_TEXTURE_2D, m_textureObject);
		//glUniform1i(m_textureIndex, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_indexBuffer);
		glDrawElements(GL_TRIANGLES,m_numFaces*3,GL_UNSIGNED_SHORT,NULL);
	m_shader->unbind();

	glDisableVertexAttribArray(m_positionIndex);
	glDisableVertexAttribArray(m_uvIndex);
#else
	_drawQuad();
#endif
}


void Water::Touch(int x, int y)
{

}

void Water::_initShader()
{
	const char vShaderStr[] =  
		"attribute vec4 vPosition;		\n"
		"attribute vec2 vTexCoord;		\n"
		"uniform vec2 v_offset;		    \n"
		"varying vec2 v_texCoord;		\n"
		"void main()					\n"
		"{								\n"
		"   gl_Position = vPosition + vec4(v_offset, 0.0,0.0); \n"
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


	m_shader = new Shader(vShaderStr,fShaderStr);

	m_positionIndex = m_shader->getAttribLocation(CTHASH("vPosition"));
	m_uvIndex = m_shader->getAttribLocation(CTHASH("vTexCoord"));
	m_textureIndex = m_shader->getUniformLocation(CTHASH("s_texture"));

	//quad shader
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

		m_quadShader = new Shader(vShaderStr, fShaderStr);
	}
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

	m_pingTexture = new Texture2D(GLuint(m_resWidth),GLuint(m_resHeight),GL_RGBA,GL_FLOAT);
	m_pangTexture = new Texture2D(GLuint(m_resWidth),GLuint(m_resHeight),GL_RGBA,GL_FLOAT);
}


void Water::_initMesh()
{
	#if 0
	m_resWidth = int(m_screenWidth/m_dx+0.5f) + 1;
	m_resHeight = int(m_screenHeight/m_dx+0.5f) + 1;
	#else
	m_resWidth = 128;
	m_resHeight = 64;
	#endif
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

	//quad buffer
	{
		WaterVertex quadVertexBuffer[] = 
		{
			WaterVertex(-1.0f,	1.0f,	0.5f,	0.0f,	0.0f),
			WaterVertex(1.0f,	1.0f,	0.5f,	1.0f,	0.0f),
			WaterVertex(-1.0f,	-1.0f,	0.5f,	0.0f,	1.0f),
			WaterVertex(1.0f,	-1.0f,	0.5f,	1.0f,	1.0f)
		};

		GLushort quadIndexBuffer[6] = {0,1,2,2,1,3};

		glGenBuffers(1,&m_quadVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,m_quadVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER,sizeof(WaterVertex)*4, quadVertexBuffer,GL_STATIC_DRAW);

		glGenBuffers(1,&m_quadIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_quadIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(quadIndexBuffer),quadIndexBuffer,GL_STATIC_DRAW);
	}
}


void Water::_drawQuad()
{

	glViewport(0,0,m_screenWidth,m_screenHeight);

	m_quadShader->bind();
		glBindBuffer(GL_ARRAY_BUFFER,m_quadVertexBuffer);
		GLint positionIndex = m_quadShader->getAttribLocation(CTHASH("vPosition"));
		GLint uvIndex = m_quadShader->getAttribLocation(CTHASH("vTexCoord"));

		glVertexAttribPointer(positionIndex,3,GL_FLOAT,0,sizeof(WaterVertex),NULL);
		unsigned int* uvOffset = reinterpret_cast<unsigned int*>(12);
		glVertexAttribPointer(uvIndex,2,GL_FLOAT,0,sizeof(WaterVertex),uvOffset);

		glEnableVertexAttribArray(positionIndex);
		glEnableVertexAttribArray(uvIndex);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textureObject);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_quadIndexBuffer);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,NULL);
	m_quadShader->unbind();

	glDisableVertexAttribArray(positionIndex);
	glDisableVertexAttribArray(uvIndex);
}
