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
	,m_screenScaleX(0.0f)
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
	//this->_initTexture();

	m_frameBufferA = new FrameBuffer(512,512, EFBT_TEXTURE_RGBA8|EFBT_TEXTURE_DEPTH);
	m_frameBufferB = new FrameBuffer(512,512, EFBT_TEXTURE_RGBA8|EFBT_TEXTURE_DEPTH);

	m_screenScaleX = m_screenWidth*1.0f/m_screenHeight;
}


void Water::Update()
{
	this->_doUpdate();
	this->_updateNormal();
}

void Water::Render()
{
	_drawQuad();
}

void Water::_processTouch(int x, int y)
{

	static float radius = 0.5f;
	static float strength = 1.0f;

	glViewport(0,0,m_frameBufferA->GetWidth(),m_frameBufferA->GetHeight());
	m_frameBufferA->Begin();
	m_shader_drop->bind();
	kmVec2 vec2;
	vec2.x = float(x)/m_screenWidth;
	vec2.y = float(y)/m_screenHeight;
	m_shader_drop->uniform(RTHASH("center"), vec2);
	m_shader_drop->uniform(RTHASH("radius"), radius);
	m_shader_drop->uniform(RTHASH("strength"), strength);
	m_shader_drop->uniform(RTHASH("scaleX"), m_screenScaleX);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_frameBufferB->GetColorTexture());
	_renderMesh(m_screenRect,m_shader_drop);
	m_shader_drop->unbind();
	m_frameBufferA->End();

	m_frameBufferA->Swap(m_frameBufferB);

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

	//normal shader
	{
		const char* strVertexShader = 
		#include "VertexShader_Common.h"
		const char* strFragmentShader_normal = 
		#include "FragmentShader_Normal.h"
		m_shader_normal = new Shader(strVertexShader, strFragmentShader_normal);
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
	static float radius = 0.5f;
	static float strength = 1.0f;

	glViewport(0,0,m_frameBufferA->GetWidth(),m_frameBufferA->GetHeight());
	m_frameBufferA->Begin();
	m_shader_drop->bind();
	kmVec2 vec2;
	vec2.x = 0.5f;
	vec2.y = 0.5f;
	m_shader_drop->uniform(RTHASH("center"), vec2);
	m_shader_drop->uniform(RTHASH("radius"), radius);
	m_shader_drop->uniform(RTHASH("strength"), strength);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_frameBufferB->GetColorTexture());
	glBindTexture(GL_TEXTURE_2D,m_textureObject);
	_renderMesh(m_screenRect,m_shader_drop);
	m_shader_drop->unbind();
	m_frameBufferA->End();
#endif

	
	glViewport(0,0,m_screenWidth,m_screenHeight);
	m_quadShader->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_frameBufferB->GetColorTexture());
	_renderMesh(m_screenRect,m_quadShader);
	m_quadShader->unbind();
	glGetError();
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

void Water::_doUpdate()
{
	static const float inverseWidth = 1.0f/m_frameBufferA->GetWidth();
	static const float inverseHeight = 1.0f/m_frameBufferA->GetHeight();

	glViewport(0,0,m_frameBufferA->GetWidth(),m_frameBufferA->GetHeight());
	m_frameBufferA->Begin();
	m_shader_update->bind();
	kmVec2 delta;
	delta.x = inverseWidth;
	delta.y = m_screenScaleX*inverseHeight;
	m_shader_update->uniform(RTHASH("delta"), delta);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_frameBufferB->GetColorTexture());
	_renderMesh(m_screenRect,m_shader_update);
	m_shader_update->unbind();
	m_frameBufferA->End();

	m_frameBufferA->Swap(m_frameBufferB);
}

void Water::_updateNormal()
{
	static const float inverseWidth = 1.0f/m_frameBufferA->GetWidth();
	static const float inverseHeight = 1.0f/m_frameBufferA->GetHeight();

	glViewport(0,0,m_frameBufferA->GetWidth(),m_frameBufferA->GetHeight());
	m_frameBufferA->Begin();
	m_shader_normal->bind();
	kmVec2 delta;
	delta.x = inverseWidth;
	delta.y = inverseHeight;
	m_shader_normal->uniform(RTHASH("delta"), delta);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_frameBufferB->GetColorTexture());
	_renderMesh(m_screenRect,m_shader_normal);
	m_shader_normal->unbind();
	m_frameBufferA->End();

	m_frameBufferA->Swap(m_frameBufferB);
}
