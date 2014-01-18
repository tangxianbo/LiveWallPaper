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
	,m_vertexBuffer(NULL)
	,m_indexBuffer(NULL)
	,m_textureObject(NULL)
	,m_positionIndex(-1)
	,m_uvIndex(-1)
	,m_heightMapIndex(-1)
	,m_textureIndex(-1)
	,m_pingTexture(nullptr)
	,m_pangTexture(nullptr)
	,m_frameBufferA(nullptr)
	,m_frameBufferB(nullptr)
	,m_shader_drop(nullptr)
	,m_shader_update(nullptr)
{
}

Water::~Water()
{

}
GLuint testTexture = 0;
void Water::Init()
{
	const GLubyte* extension = glGetString(GL_EXTENSIONS);

	this->_initShader();
	this->_initMesh();
	this->_initTexture();

	//just for test
	/*
	GLubyte pixels[4*3] = 
	{
		255, 0, 0,
		0,	255,	0,
		0,	0,	255,
		255,	255,	0
	};

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &testTexture);
	glBindTexture(GL_TEXTURE_2D, testTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	*/
	//test end

	m_frameBufferA = new FrameBuffer(m_resWidth,m_resHeight, EFBT_TEXTURE_RGB8|EFBT_TEXTURE_DEPTH|EFBT_TEXTURE_WHITE);
	m_frameBufferB = new FrameBuffer(m_resWidth,m_resHeight, EFBT_TEXTURE_RGB8|EFBT_TEXTURE_DEPTH|EFBT_TEXTURE_WHITE);


}


void Water::Update()
{
	while (m_touchQueue.size() > 0)
	{
		TouchPos touch = m_touchQueue.front();
		m_touchQueue.pop();
		this->_processTouch(touch.X, touch.Y);
	}
}

void Water::Render()
{
	while(!m_touchQueue.empty())
	{
		auto touch = m_touchQueue.front();

	}

	_drawQuad();
}

void Water::_processTouch(int x, int y)
{
	static float radius = 3.0f;
	static float strength = 1.0f;

	//use drop shader 
	//render to texture a
	//display texture a

	/*
	glViewport(0,0,m_screenWidth,m_screenHeight);
	m_frameBufferA->Begin();
	m_shader_drop->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_textureObject);
	_renderMesh(m_screenRect,m_shader_drop);
	m_shader_drop->unbind();
	m_frameBufferA->End();
	*/

	/*
	glViewport(0,0,m_screenWidth,m_screenHeight);
	m_quadShader->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_frameBufferA->GetColorTexture());
	_renderMesh(m_screenRect,m_quadShader);
	m_quadShader->unbind();
	*/

}

void Water::_initShader()
{
	//quad shader
	{
		const char* strVertexShader = 
		#include "VertexShader_Quad.h"
		const char* strFragmentShader = 
		#include "FragmentShader_Quad.h"
		m_quadShader = new Shader(strVertexShader, strFragmentShader);
	}

	//drop shader 
	{
		const char* vertexShader = 
		#include "VertexShader_Common.h"
		const char* fragmentShader = 
		#include "FragmentShader_Drop.h"
		m_shader_drop = new Shader(vertexShader,fragmentShader);
	}

	//update shader
	{
		const char* strVertexShader = 
		#include "VertexShader_Common.h"
		const char* strFragmentShader = 
		#include "FragmentShader_Update.h"
		m_shader_update = new Shader(strVertexShader,strFragmentShader);
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

	//m_pingTexture = new Texture2D(GLuint(m_screenWidth),GLuint(m_screenHeight),GL_RGBA,GL_FLOAT);
	//m_pangTexture = new Texture2D(GLuint(m_screenWidth),GLuint(m_screenHeight),GL_RGBA,GL_FLOAT);
}


void Water::_initMesh()
{
	#if 0
	m_resWidth = int(m_screenWidth/m_dx+0.5f) + 1;
	m_resHeight = int(m_screenHeight/m_dx+0.5f) + 1;
	#else
	m_resWidth = 128;
	m_resHeight = 128;
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

		m_screenRect = new MeshObject(m_quadVertexBuffer,m_quadIndexBuffer);
		m_screenRect->addMeshAttribute("position",3,GL_FLOAT,sizeof(WaterVertex),0);
		m_screenRect->addMeshAttribute("coord",2,GL_FLOAT,sizeof(WaterVertex),12);
		m_screenRect->setIndexCount(6);
	}
}


void Water::_drawQuad()
{
#if 0
	glViewport(0,0,m_screenWidth,m_screenHeight);
	m_quadShader->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureObject);
	_renderMesh(m_screenRect,m_quadShader);
	m_quadShader->unbind();
#else
#if 0
	glViewport(0,0,m_frameBufferA->GetWidth(),m_frameBufferA->GetHeight());
	m_frameBufferA->Begin();
	m_quadShader->bind();
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_textureObject);
	_renderMesh(m_screenRect,m_quadShader);
	m_quadShader->unbind();
	m_frameBufferA->End();
#else
	
	static float radius = 3.0f;
	static float strength = 1.0f;

	glViewport(0,0,m_frameBufferA->GetWidth(),m_frameBufferA->GetHeight());
	//glViewport(0,0,m_screenWidth,m_screenHeight);
	m_frameBufferA->Begin();
	m_shader_drop->bind();
	kmVec2 vec2;
	vec2.x = 200.0f;
	vec2.y = 100.0f;
	m_shader_drop->uniform(RTHASH("center"), vec2);
	m_shader_drop->uniform(RTHASH("radius"), radius);
	m_shader_drop->uniform(RTHASH("strength"), strength);
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D,m_frameBufferB->GetColorTexture());
	glBindTexture(GL_TEXTURE_2D,m_textureObject);
	_renderMesh(m_screenRect,m_shader_drop);
	m_shader_drop->unbind();
	m_frameBufferA->End();
	
#endif

	glViewport(0,0,m_screenWidth,m_screenHeight);
	m_quadShader->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_frameBufferA->GetColorTexture());
	//glBindTexture(GL_TEXTURE_2D,m_textureObject);
	//glBindTexture(GL_TEXTURE_2D,testTexture);
	_renderMesh(m_screenRect,m_quadShader);
	m_quadShader->unbind();
	//glBindTexture(GL_TEXTURE_2D, 0);

	glGetError();
#endif
}

void Water::_renderMesh(const MeshObject* mesh, const Shader* shader)
{
    GLuint vbo = mesh->getVBO();
    GLuint ibo = mesh->getIBO();

    glBindBuffer(GL_ARRAY_BUFFER,vbo);

    for (auto it = shader->getVertexAttributesBegin(); 
        it != shader->getVertexAttributesEnd(); 
        ++it)
    {

        E_Vertex_Attribute attribute = it->attributeType;
        s32  attributesLoc = it->location;
        const MeshAttributeDef* meshAttribute = mesh->getMeshAttribute(attribute);

        glVertexAttribPointer(attributesLoc,
                              meshAttribute->m_elementNum,
                              meshAttribute->m_elementType,
                              0,
                              meshAttribute->m_stride, 
                              reinterpret_cast<u32*>(meshAttribute->m_offset)
                              );

        glEnableVertexAttribArray(attributesLoc);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
    glDrawElements(GL_TRIANGLES, 
                   mesh->getIndexCount(),
                   GL_UNSIGNED_SHORT,
                   NULL);

    for (auto it = shader->getVertexAttributesBegin(); 
        it != shader->getVertexAttributesEnd(); 
        ++it)
    {
        glDisableVertexAttribArray(it->location);
    }
}
