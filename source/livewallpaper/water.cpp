#include "water.h"
#include "esutils.h"
#include <vector>
#include <ktx.h>
#include "texture2d.h"
#include "framebuffer.h"
#include <string>
#include "shader.h"
#include <core/string_hash.h>
#include <math/math.h>
#include <math/vector2d.h>
#include <math/vector3d.h>
#include <math/vector4d.h>
#include <math/matrix3.h>
#include <math/matrix4.h>
#include <math/quaternion.h>
#include "esutils.h"

using namespace jenny;

//temp code

vector2di getSceenDPI()
{
	HDC screen = GetDC(0);
	int dpiX = GetDeviceCaps (screen, LOGPIXELSX);
	int dpiY = GetDeviceCaps (screen, LOGPIXELSY);
	ReleaseDC (0, screen);

	return vector2di(dpiX, dpiY);
}

Water::Water(int screenWidth, int screenHeight, float dx):	m_screenWidth(screenWidth)
	,m_screenHeight(screenHeight)
	,m_indexEleNum(0)
	,m_screenScaleX(0.0f)
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
	,m_frameBufferCaustic(nullptr)
	,m_shader_drop(nullptr)
	,m_shader_update(nullptr)
	,m_shader_water(nullptr)
	,m_shader_caustics(nullptr)
	,m_shader_waterMesh(nullptr)
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

	m_frameBufferA = new FrameBuffer(512,512, EFBT_TEXTURE_RGBA8|EFBT_TEXTURE_DEPTH);
	m_frameBufferB = new FrameBuffer(512,512, EFBT_TEXTURE_RGBA8|EFBT_TEXTURE_DEPTH);
	m_frameBufferCaustic = new FrameBuffer(512, 512, EFBT_TEXTURE_RGBA8|EFBT_TEXTURE_DEPTH); 

	m_screenScaleX = m_screenWidth*1.0f/m_screenHeight;
}


void Water::Update()
{
#if 1
	this->_doUpdate();
	this->_updateNormal();
	this->_genCaustics();
#endif
}

void Water::Render()
{
#if 1
	_drawQuad();
#else
	_drawWaterMesh();
#endif
}

void Water::_processTouch(int x, int y)
{

	static float radius = 0.2f;
	static float strength = 1.0f;

	glViewport(0,0,m_frameBufferA->GetWidth(),m_frameBufferA->GetHeight());
	m_frameBufferA->Begin();
	m_shader_drop->bind();
	vector2df vec2(float(x)/m_screenWidth,float(y)/m_screenHeight);
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

	//water shader
	{
		const char* strVertexShader = 
		#include "VertexShader_Water.h"
		const char* strFragmentShader =
		#include "FragmentShader_Water.h"
		m_shader_water = new Shader(strVertexShader, strFragmentShader);
	}

	//caustic shader
	{
		const char* strVertexShader =
		#include "VertexShader_Caustic.h"
		const char* strFragmentShader =
		#include "FragmentShader_Caustic.h"
		m_shader_caustics = new Shader(strVertexShader,strFragmentShader); 
	}

	//water mesh
	{
		const char* strVertexShader =
		#include "VertexShader_WaterMesh.h"
		const char* strFragmentShader =
		#include "FragmentShader_WaterMesh.h"
		m_shader_waterMesh = new Shader(strVertexShader, strFragmentShader);
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
	//water mesh
	{
		static const float VERTEX_PER_INCH =  50.0f;

	#if 0
		vector2di dpi = getSceenDPI();
		int resWidth = int(float(m_screenWidth)/dpi.getX()*VERTEX_PER_INCH + 0.5f);
		int resHeight = int(float(m_screenHeight)/dpi.getY()*VERTEX_PER_INCH + 0.5f);
	#else
		int resWidth = 200;
		int resHeight = 200;
	#endif

		vector3df* vertexBuffer = new vector3df[resWidth*resHeight];

		float inverseWidth = 1.0f/(resWidth-1);
		float inverseHeight = 1.0f/(resHeight-1);
		for (int y=0; y<resHeight; ++y)
		{
			float posY = y*inverseHeight;
			for (int x=0; x<resWidth; ++x)
			{
				float posX = x*inverseWidth;
				vertexBuffer[y*resWidth + x].set(posX, posY, 0.0f);
			}
		}


		int numFaces = (resWidth-1)*(resHeight-1)*2;
		std::vector<GLushort> indices(numFaces*3);
		int k = 0;
		for (int y=0; y<resHeight-1; ++y)
		{
			for(int x=0; x<resWidth-1; ++x)
			{
				indices[k++] = y*resWidth + x;
				indices[k++] = y*resWidth + x + 1;
				indices[k++] = (y + 1)*resWidth + x;

				indices[k++] = (y + 1)*resWidth + x;
				indices[k++] = y*resWidth + x + 1;
				indices[k++] = (y + 1)*resWidth + x + 1;
			}
		}

		glGenBuffers(1,&m_vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,m_vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER,sizeof(vector3df)*resWidth*resHeight, vertexBuffer,GL_STATIC_DRAW);

		glGenBuffers(1,&m_indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLushort)*numFaces*3,&indices[0],GL_STATIC_DRAW);

		m_waterMesh = new MeshObject(m_vertexBuffer,m_indexBuffer);
		m_waterMesh->addMeshAttribute("position",3,GL_FLOAT,sizeof(vector3df),0);
		m_waterMesh->setIndexCount(numFaces*3);

		delete[] vertexBuffer;
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

	//test triangle
	{
		vector3df vertexBufferData[] = 
		{
			vector3df(0.0f, 0.0f, 0.0f),
			vector3df(3.0f, 0.0f, 0.0f),
			vector3df(0.0f, 3.0f, 0.0f)
		};

		GLushort indexBufferData[3] = {0,1,2};
		GLuint vertexBuffer, indexBuffer;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vector3df)*3, vertexBufferData, GL_STATIC_DRAW);

		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBufferData), indexBufferData, GL_STATIC_DRAW);

		m_testTriangle = new MeshObject(vertexBuffer, indexBuffer);
		m_testTriangle->addMeshAttribute("position", 3, GL_FLOAT, sizeof(vector3df), 0);
		m_testTriangle->setIndexCount(3);
	}
}


extern matrix4 g_viewProjectMatrix;
extern matrix4 g_viewProjectMatrixOrc;
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

#if 0
	glViewport(0,0,m_screenWidth,m_screenHeight);
	m_quadShader->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_frameBufferB->GetColorTexture());
	_renderMesh(m_screenRect,m_quadShader);
	m_quadShader->unbind();
	glGetError();

	//
#else

	vector4df transformedVertexs[3];
	vector4df originalVertex[] = 
	{
		vector4df(0.0f, 0.0f, 0.5f, 1.0f),
		vector4df(1.0f, 0.0f, 0.5f, 1.0f),
		vector4df(0.0f, 0.0f, -1.0f, 1.0f)
	};

	for (int i=0; i<3; ++i)
	{
		g_viewProjectMatrix.transformVect(transformedVertexs[i],originalVertex[i]);
	}

	matrix4 transposedMatrix = g_viewProjectMatrix.getTransposed();
	vector2df screenSize((float)m_screenWidth, (float)m_screenHeight);
	//vector2df screenSize(10.0f, 10.0f);

	glViewport(0, 0, m_screenWidth, m_screenHeight); 
	m_shader_water->bind();
	glActiveTexture(GL_TEXTURE0);
#if 1
	glBindTexture(GL_TEXTURE_2D, m_frameBufferB->GetColorTexture());
#else
	glBindTexture(GL_TEXTURE_2D, m_frameBufferCaustic->GetColorTexture());
#endif

	m_shader_water->uniform(RTHASH("screenSize"), screenSize);

#if 1
	m_shader_water->uniform(RTHASH("WVPMatrix"), g_viewProjectMatrixOrc);
#else
	m_shader_water->uniform(RTHASH("WVPMatrix"), g_viewProjectMatrix);
#endif
	//_renderMesh(m_testTriangle, m_shader_water);
	_renderMesh(m_waterMesh,m_shader_water);
	m_shader_water->unbind();
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

void Water::_doUpdate()
{
	static const float inverseWidth = 1.0f/m_frameBufferA->GetWidth();
	static const float inverseHeight = 1.0f/m_frameBufferA->GetHeight();

	glViewport(0,0,m_frameBufferA->GetWidth(),m_frameBufferA->GetHeight());
	m_frameBufferA->Begin();
	m_shader_update->bind();
	vector2df delta(inverseWidth, m_screenScaleX*inverseHeight);
	//delta.x = inverseWidth;
	//delta.y = m_screenScaleX*inverseHeight;
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
	vector2df delta(inverseWidth, inverseHeight);
	//delta.x = inverseWidth;
	//delta.y = inverseHeight;
	m_shader_normal->uniform(RTHASH("delta"), delta);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_frameBufferB->GetColorTexture());
	_renderMesh(m_screenRect,m_shader_normal);
	m_shader_normal->unbind();
	m_frameBufferA->End();

	m_frameBufferA->Swap(m_frameBufferB);
}

void Water::_genCaustics()
{
	glViewport(0, 0, m_frameBufferCaustic->GetWidth(), m_frameBufferCaustic->GetHeight());
	m_frameBufferCaustic->Begin();
	glClear ( GL_COLOR_BUFFER_BIT );
	m_shader_caustics->bind();

	//uniform screensize
	vector2df screenSize((float)m_screenWidth, (float)m_screenHeight);
	m_shader_caustics->uniform(RTHASH("screenSize"), screenSize);

	//uniform light dir
	vector3df light(2.0f, -1.0f, 2.0f); //light(0.5f, 0.0f, 1.0f);
	light.normalize();
	m_shader_caustics->uniform(RTHASH("light"), light);

	//uniform texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_frameBufferB->GetColorTexture());

	_renderMesh(m_waterMesh,m_shader_caustics);

	m_shader_caustics->unbind();
	m_frameBufferCaustic->End();
}


void Water::_drawWaterMesh()
{
	glViewport(0, 0, m_screenWidth, m_screenHeight); 
	m_shader_waterMesh ->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_frameBufferB->GetColorTexture());

	vector2df screenSize((float)m_screenWidth, (float)m_screenHeight);
	m_shader_waterMesh->uniform(RTHASH("screenSize"), screenSize);

	vector3df light(2.0f, -1.0f, 2.0f); //light(0.5f, 0.0f, 1.0f);
	light.normalize();
	m_shader_waterMesh->uniform(RTHASH("light"), light);
#if 1
	m_shader_waterMesh->uniform(RTHASH("WVPMatrix"), g_viewProjectMatrixOrc);
#else
	m_shader_waterMesh->uniform(RTHASH("WVPMatrix"), g_viewProjectMatrix);
#endif

	_renderMesh(m_waterMesh,m_shader_waterMesh);
	m_shader_waterMesh->unbind();
	glGetError();
}
