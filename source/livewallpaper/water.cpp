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
#include "Renderer.h"


Water::Water(int screenWidth, int screenHeight, float dx):	m_screenWidth(screenWidth)
	,m_screenHeight(screenHeight)
	,m_resWidth(0)
	,m_resHeight(0)
	,m_dx(dx)
	,m_vertexBufferSize(0)
	,m_indexEleNum(0)
	,m_curVertexBuffer(nullptr)
	,m_preVertexBuffer(nullptr)
	,m_backgroundTexture(NULL)
	,m_pingTexture(nullptr)
	,m_pangTexture(nullptr)
{
}

Water::~Water()
{
	delete m_quadMesh;
	delete m_quadShader;
}

void Water::Init()
{
	const GLubyte* extension = glGetString(GL_EXTENSIONS);

	this->_initShader();
	this->_initMesh();
	this->_initTexture();

	m_frameBuffer = new FrameBuffer(m_screenWidth,m_screenHeight);
}

void Water::Render()
{
	_drawQuad();
}


void Water::Touch(int x, int y)
{

}

void Water::_initShader()
{
	//quad shader
	{
		const char vShaderStr[] =  
			"attribute vec4 position;		\n"
			"attribute vec2 texcoord;		\n"
			"varying vec2 v_texCoord;		\n"
			"void main()					\n"
			"{								\n"
			"   gl_Position = position;		\n"
			"   v_texCoord = texcoord;		\n"	
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

		ktxerror = ktxLoadTextureM(fileData,fileSize,&m_backgroundTexture,&target,NULL,&isMipmapped,&glerror,0,NULL);
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

		GLuint m_vertexBuffer = 0, m_indexBuffer = 0;
		glGenBuffers(1,&m_vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,m_vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER,m_vertexBufferSize, m_curVertexBuffer,GL_STATIC_DRAW);

		glGenBuffers(1,&m_indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLushort)*m_numFaces*3,&indices[0],GL_STATIC_DRAW);

		//TODO.
	}


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

		GLuint vertexBuffer = 0, indexBuffer = 0;
		glGenBuffers(1,&vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER,sizeof(WaterVertex)*4, quadVertexBuffer,GL_STATIC_DRAW);

		glGenBuffers(1,&indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(quadIndexBuffer),quadIndexBuffer,GL_STATIC_DRAW);

		m_quadMesh = new MeshObject(vertexBuffer,indexBuffer);
		m_quadMesh->addMeshAttribute("position",3,GL_FLOAT,sizeof(WaterVertex),0);
		m_quadMesh->addMeshAttribute("coord",2,GL_FLOAT,sizeof(WaterVertex),12);
		m_quadMesh->setIndexCount(6);
	}
}


void Water::_drawQuad()
{
	m_frameBuffer->Begin();
	m_quadShader->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_backgroundTexture);
	Renderer::RenderMesh(m_quadMesh,m_quadShader);
	m_quadShader->unbind();
	m_frameBuffer->End();

	m_quadShader->bind();
	m_frameBuffer->bindColorTexture(GL_TEXTURE0);
	Renderer::RenderMesh(m_quadMesh,m_quadShader);
	m_quadShader->unbind();
}
