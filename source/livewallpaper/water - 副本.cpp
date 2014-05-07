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
	,m_fbWrite(nullptr)
	,m_fbRead(nullptr)
	,m_frameBufferCaustic(nullptr)
	,m_shader_drop(nullptr)
	,m_shader_update(nullptr)
	,m_shader_water(nullptr)
	,m_shader_caustics(nullptr)
	,m_shader_waterMesh(nullptr)
	,m_shader_init(nullptr)
	,m_shader_water_uv(nullptr)
{
}

Water::~Water()
{

}

void Water::Init()
{
	const GLubyte* extension = glGetString(GL_EXTENSIONS);

	this->_initShader();
	//this->_initMesh();
	this->_initWaterMeshUV();
	this->_initTexture();

#if 1
	m_fbWrite = new FrameBuffer(512,512, EFBT_TEXTURE_RGBA8|EFBT_TEXTURE_DEPTH);
	m_fbRead = new FrameBuffer(512,512, EFBT_TEXTURE_RGBA8|EFBT_TEXTURE_DEPTH);
	m_frameBufferCaustic = new FrameBuffer(512, 512, EFBT_TEXTURE_RGBA8|EFBT_TEXTURE_DEPTH); 
#endif

	//this->_initFrameBuffers();

	m_screenScaleX = m_screenWidth*1.0f/m_screenHeight;
}


void Water::Update()
{
	this->_updateWaterMeshUV();

#if 1
	//this->_doUpdate();
	//this->_doUpdate();
	//this->_updateNormal();
	//this->_genCaustics();
#endif
}

void Water::Render()
{
	this->_drawWaterMeshUV();
#if 0
	//_drawQuad();
#else
	//_drawWaterMesh();
#endif
}

void Water::_processTouch(int x, int y)
{
#if 0
	static float radius = 0.1f;
	static float strength = 0.6f;

	glViewport(0,0,m_fbWrite->GetWidth(),m_fbWrite->GetHeight());
	m_fbWrite->Begin();
	m_shader_drop->bind();
	vector2df vec2(float(x)/m_screenWidth,float(y)/m_screenHeight);
	m_shader_drop->uniform(RTHASH("center"), vec2);
	m_shader_drop->uniform(RTHASH("radius"), radius);
	m_shader_drop->uniform(RTHASH("strength"), strength);
	m_shader_drop->uniform(RTHASH("scaleX"), m_screenScaleX);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_fbRead->GetColorTexture());
	_renderMesh(m_screenRect,m_shader_drop);
	m_shader_drop->unbind();
	m_fbWrite->End();

	m_fbWrite->Swap(m_fbRead);
#endif
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

	//init shader
	{
		const char* strVertexShader =
		#include "VertexShader_Common.h"
		const char* strFragmentShader = 
		#include "FragmentShader_Init.h"
		m_shader_init = new Shader(strVertexShader, strFragmentShader);
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
#if 0
	{
		const char* strVertexShader = 
		#include "VertexShader_Common.h"
		const char* strFragmentShader_normal = 
		#include "FragmentShader_Normal.h"
		m_shader_normal = new Shader(strVertexShader, strFragmentShader_normal);
	}
#endif

	//water shader
	{
		const char* strVertexShader = 
		#include "VertexShader_Water.h"
		const char* strFragmentShader =
		#include "FragmentShader_Water.h"
		m_shader_water = new Shader(strVertexShader, strFragmentShader);
	}

	//caustic shader
#if 0
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
#endif

	//water mesh uv
	{
		const char* strVertexShader =
		#include "VertexShader_Water_UV.h"
		const char* strFragmentShader =
		#include "FragmentShader_Water_UV.h"
		m_shader_water_uv = new Shader(strVertexShader, strFragmentShader);
	}
}

void Water::_initTexture()
{
	GLenum		target;
	GLenum		glerror;
	GLboolean	isMipmapped;
	KTX_error_code ktxerror;

	FILE* ktxFile = fopen("grid.ktx","rb");
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
		int resWidth = 256;
		int resHeight = 256;
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
	//glBindTexture(GL_TEXTURE_2D, m_frameBufferB->GetColorTexture());
	glBindTexture(GL_TEXTURE_2D, m_textureObject);
	m_quadShader->uniform(RTHASH("s_texture"), 0);
	_renderMesh(m_screenRect,m_quadShader);
	m_quadShader->unbind();
	glGetError();

	//
#else

	//vector4df transformedVertexs[3];
	//vector4df originalVertex[] = 
	//{
	//	vector4df(0.0f, 0.0f, 0.5f, 1.0f),
	//	vector4df(1.0f, 0.0f, 0.5f, 1.0f),
	//	vector4df(0.0f, 0.0f, -1.0f, 1.0f)
	//};

	//for (int i=0; i<3; ++i)
	//{
	//	g_viewProjectMatrix.transformVect(transformedVertexs[i],originalVertex[i]);
	//}

	matrix4 transposedMatrix = g_viewProjectMatrix.getTransposed();
	vector2df screenSize((float)m_screenWidth, (float)m_screenHeight);
	//vector2df screenSize(10.0f, 10.0f);

	//
	static const float inverseWidth = 1.0f/m_fbWrite->GetWidth();
	static const float inverseHeight = 1.0f/m_fbWrite->GetHeight();
	//

	glViewport(0, 0, m_screenWidth, m_screenHeight); 
	m_shader_water->bind();
	glActiveTexture(GL_TEXTURE0);
#if 1
	glBindTexture(GL_TEXTURE_2D, m_fbRead->GetColorTexture());
#else
	glBindTexture(GL_TEXTURE_2D, m_frameBufferCaustic->GetColorTexture());
#endif
	m_shader_water->uniform(RTHASH("water"),0);

#if 0
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_textureObject);
	m_shader_water->uniform(RTHASH("base"),1);
#endif

	m_shader_water->uniform(RTHASH("screenSize"), screenSize);

#if 1
	m_shader_water->uniform(RTHASH("WVPMatrix"), g_viewProjectMatrixOrc);
#else
	m_shader_water->uniform(RTHASH("WVPMatrix"), g_viewProjectMatrix);
#endif
	vector2df delta(inverseWidth, inverseHeight);
	m_shader_water->uniform(RTHASH("delta"), delta);

	_renderMesh(m_screenRect,m_shader_water);
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
	static const float inverseWidth = 1.0f/m_fbWrite->GetWidth();
	static const float inverseHeight = 1.0f/m_fbWrite->GetHeight();

	glViewport(0,0,m_fbWrite->GetWidth(),m_fbWrite->GetHeight());
	m_fbWrite->Begin();
	m_shader_update->bind();
	vector2df delta(inverseWidth, m_screenScaleX*inverseHeight);
	//delta.x = inverseWidth;
	//delta.y = m_screenScaleX*inverseHeight;
	m_shader_update->uniform(RTHASH("delta"), delta);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_fbRead->GetColorTexture());
	m_shader_update->uniform(RTHASH("texture"), 0);
	_renderMesh(m_screenRect,m_shader_update);
	m_shader_update->unbind();
	m_fbWrite->End();

	m_fbWrite->Swap(m_fbRead);
}

void Water::_updateNormal()
{
	static const float inverseWidth = 1.0f/m_fbWrite->GetWidth();
	static const float inverseHeight = 1.0f/m_fbWrite->GetHeight();

	glViewport(0,0,m_fbWrite->GetWidth(),m_fbWrite->GetHeight());
	m_fbWrite->Begin();
	m_shader_normal->bind();
	vector2df delta(inverseWidth, inverseHeight);
	//delta.x = inverseWidth;
	//delta.y = inverseHeight;
	m_shader_normal->uniform(RTHASH("delta"), delta);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_fbRead->GetColorTexture());
	_renderMesh(m_screenRect,m_shader_normal);
	m_shader_normal->unbind();
	m_fbWrite->End();

	m_fbWrite->Swap(m_fbRead);
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
	glBindTexture(GL_TEXTURE_2D,m_fbRead->GetColorTexture());

	_renderMesh(m_waterMesh,m_shader_caustics);

	m_shader_caustics->unbind();
	m_frameBufferCaustic->End();
}


void Water::_drawWaterMesh()
{
	glViewport(0, 0, m_screenWidth, m_screenHeight); 
	m_shader_waterMesh ->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fbRead->GetColorTexture());

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

void Water::_initFrameBuffers()
{
	//Init read texture
	glViewport(0,0,m_fbRead->GetWidth(),m_fbRead->GetHeight());
	m_fbRead->Begin();
	m_shader_init->bind();
	_renderMesh(m_screenRect,m_shader_init);
	m_shader_init->unbind();
	m_fbRead->End();

	//Init write texture
	glViewport(0,0,m_fbWrite->GetWidth(),m_fbWrite->GetHeight());
	m_fbWrite->Begin();
	m_shader_init->bind();
	_renderMesh(m_screenRect,m_shader_init);
	m_shader_init->unbind();
	m_fbWrite->End();
}

vector2df* m_pUVBufferRead = nullptr;
vector2df* m_pUVBufferWrite = nullptr;

int *m_pHightRead = nullptr;
int *m_pHightWrite = nullptr;

int resWidth = 256;
int resHeight = 256;

void Water::_initWaterMeshUV()
{
	vector2df* vertexBuffer = new vector2df[resWidth*resHeight];
	m_pUVBufferRead = new vector2df[resWidth*resHeight];
	m_pUVBufferWrite = new vector2df[resWidth*resHeight];

	m_pHightRead = new int[resWidth*resHeight];
	m_pHightWrite = new int[resWidth*resHeight];

	float inverseWidth = 1.0f/(resWidth-1);
	float inverseHeight = 1.0f/(resHeight-1);
	for (int y=0; y<resHeight; ++y)
	{
		float posY = y*inverseHeight;
		for (int x=0; x<resWidth; ++x)
		{
			float posX = x*inverseWidth;
			vertexBuffer[y*resWidth + x].set((posX - 0.5f)*2.0f, (posY-0.5f)*2.0f);
			m_pUVBufferRead[y*resWidth + x].set(posX, posY);
			m_pUVBufferWrite[y*resWidth + x].set(posX, posY);

			m_pHightRead[y*resWidth + x] = 0;
			m_pHightWrite[y*resWidth + x] = 0;
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

	glGenBuffers(1,&m_vertexBuffer_Pos);
	glBindBuffer(GL_ARRAY_BUFFER,m_vertexBuffer_Pos);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vector2df)*resWidth*resHeight, vertexBuffer,GL_STATIC_DRAW);

	glGenBuffers(1,&m_indexBuffer_UV);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_indexBuffer_UV);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLushort)*numFaces*3,&indices[0],GL_STATIC_DRAW);

	glGenBuffers(1, &m_vertexBuffer_UV);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_UV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vector2df)*resWidth*resHeight, m_pUVBufferWrite, GL_DYNAMIC_DRAW);

	m_waterMesh_UV = new MeshObject(m_vertexBuffer_Pos,m_indexBuffer_UV);
	m_waterMesh_UV->addMeshAttribute("position",2,GL_FLOAT,sizeof(vector2df),0);
	m_waterMesh_UV->addMeshAttribute("coord",2,GL_FLOAT,sizeof(vector2df),0);
	m_waterMesh_UV->setIndexCount(numFaces*3);

	delete[] vertexBuffer;
}

int inline READBUFFER(int* buffer, int x, int y)
{
	return (buffer[y*resWidth + x]);
}

const float m_DampingFactor = 0.04f;
void Water::_updateWaterMeshUV()
{
	int i,j;
	float value;

	for (j=2; j<resHeight - 2; j++)
	{
		for (i=2; i< resWidth - 2; i++)
		{
			value = (float)(
				READBUFFER(m_pHightRead,i-2,j) +
				READBUFFER(m_pHightRead,i+2,j) +
				READBUFFER(m_pHightRead,i,j-2) +
				READBUFFER(m_pHightRead,i,j+2) +
				READBUFFER(m_pHightRead,i-1,j) +
				READBUFFER(m_pHightRead,i+1,j) +
				READBUFFER(m_pHightRead,i,j-1) +
				READBUFFER(m_pHightRead,i,j+1) +
				READBUFFER(m_pHightRead,i-1,j-1) +
				READBUFFER(m_pHightRead,i+1,j-1) +
				READBUFFER(m_pHightRead,i-1,j+1) +
				READBUFFER(m_pHightRead,i+1,j+1));

			value /= 6.0f;
			value -= (float)READBUFFER(m_pHightWrite, i, j);
			value -= (value*m_DampingFactor);

			m_pHightWrite[j * resWidth + i] = (int)value;

			if(int(value) != 0)
			{
				int a= 0;
				a++;
			}
		}
	}

	//swap data
	std::swap(m_pHightRead, m_pHightWrite);


	//generate uv offset
	int xoff, yoff, cnt = 0;

	for (int j=0; j< resHeight; j++)
	{
		for (int i=0; i<resWidth; i++, cnt ++)
		{
			xoff = 0;
			if(i > 0 && i < resWidth - 1)
			{
				xoff -= (m_pHightRead[cnt - 1]);
				xoff += (m_pHightRead[cnt + 1]);
			}

			yoff = 0;
			if(j>0 && j<resHeight - 1)
			{
				yoff -= m_pHightRead[cnt - resWidth];
				yoff += m_pHightRead[cnt + resWidth];
			}

			//one equals one pixel
			const vector2df& oriUV = m_pUVBufferRead[cnt];
			vector2df& newUV = m_pUVBufferWrite[cnt];
			newUV.setX(xoff*1.0f/512.0f + oriUV.getX());
			newUV.setY(yoff*1.0f/512.0f + oriUV.getY());

			if(xoff != 0)
			{
				int a = 0;
				a++;
			}
		}
	}

	//upload to gpu
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_UV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vector2df)*resWidth*resHeight, m_pUVBufferWrite, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Water::_drawWaterMeshUV()
{
	glViewport(0, 0, m_screenWidth, m_screenHeight);
	m_shader_water_uv->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureObject);
	m_shader_water->uniform(RTHASH("water"),0);

	Shader::VertexAttributeIter iter =m_shader_water_uv->getVertexAttributesBegin();
	for (; iter != m_shader_water_uv->getVertexAttributesEnd(); ++iter)
	{
		if (iter->attributeType == E_Vertex_Attribute::EVA_POSITION)
		{
			auto meshAttribute = m_waterMesh_UV->getMeshAttribute(iter->attributeType);
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_Pos);
			glEnableVertexAttribArray(iter->location);
	
			glVertexAttribPointer(iter->location,
				meshAttribute->m_elementNum,
				meshAttribute->m_elementType,
				0,
				meshAttribute->m_stride, 
				reinterpret_cast<u32*>(meshAttribute->m_offset)
				);
		}

		if(iter->attributeType == E_Vertex_Attribute::EVA_TEXCOORD0)
		{
			auto meshAttribute = m_waterMesh_UV->getMeshAttribute(iter->attributeType);
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_UV);
			glEnableVertexAttribArray(iter->location);

			glVertexAttribPointer(iter->location,
				meshAttribute->m_elementNum,
				meshAttribute->m_elementType,
				0,
				meshAttribute->m_stride, 
				reinterpret_cast<u32*>(meshAttribute->m_offset)
				);
		}
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer_UV);
	glDrawElements(GL_TRIANGLES, m_waterMesh_UV->getIndexCount(), GL_UNSIGNED_SHORT, NULL);
}

int inline SquaredDist(int sx, int sy, int dx, int dy)
{
	return ((dx - sx) * (dx - sx)) + ((dy - sy) * (dy - sy));
}

const int m_Drip_Radius = 12;
const int m_Drip_Radius_Sqr = 12*12;
void Water::_processTouchUV(int x, int y, int depth)
{
	int i,j,dist,finaldepth;

	for (j = y - m_Drip_Radius; j < y + m_Drip_Radius; j++)
	{
		for (i = x - m_Drip_Radius; i < x + m_Drip_Radius; i++)
		{
			dist = SquaredDist(x,y,i,j);
			if(dist < m_Drip_Radius_Sqr)
			{
				finaldepth = (int)((float)depth * ((float)(m_Drip_Radius - sqrt((float)dist))/(float)m_Drip_Radius));
				if(finaldepth > 127) finaldepth = 127;
				if(finaldepth < -127) finaldepth = -127;

				m_pHightWrite[j*resWidth + i] = finaldepth;
			}
		}
	}
}