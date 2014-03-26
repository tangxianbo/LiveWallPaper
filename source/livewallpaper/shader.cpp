#include "shader.h"
#include <core/string_hash.h>


Shader::Shader(const char* verStr, const char* fragStr):mShaderProgram(0)
                                                        ,m_ShaderAttributes(nullptr)
                                                        ,m_ShaderAttributesNum(0)
{
   GLuint vertShader = this->loadShader(GL_VERTEX_SHADER,verStr);
   GLuint fragShader = this->loadShader(GL_FRAGMENT_SHADER, fragStr);

   if (vertShader && fragShader)
   {
       mShaderProgram = glCreateProgram();
       if(mShaderProgram)
       {
           glAttachShader(mShaderProgram,vertShader);
           glAttachShader(mShaderProgram,fragShader);

           glLinkProgram(mShaderProgram);

           GLint linked;
           glGetProgramiv(mShaderProgram,GL_LINK_STATUS,&linked);

		   if(linked)
		   {
			   generateShaderInfo(mShaderProgram);
		   }
		   else
           {
               glDeleteProgram(mShaderProgram);
               mShaderProgram = 0;
           }
       }
   }

   glDeleteShader(vertShader);
   glDeleteShader(fragShader);

   
}

Shader::~Shader()
{
	glDeleteProgram(mShaderProgram);
	delete[] reinterpret_cast<char*>(m_ShaderAttributes);
	delete[] reinterpret_cast<char*>(mShaderUniformsInfo.crbegin()->second);
}


GLuint 
Shader::loadShader ( GLenum type, const char* shaderStr)
{
    GLuint shader;
    GLint compiled;

    shader = glCreateShader ( type );
    if ( shader == 0 )
        return 0;

    glShaderSource ( shader, 1, &shaderStr, NULL );
    glCompileShader ( shader );
    glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

    if ( !compiled ) 
    {
        GLint infoLen = 0;
		GLint len = 0;
        glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
        if ( infoLen > 1 )
        {
            char* infoLog = reinterpret_cast<char*>(malloc(sizeof(char) * infoLen));
            glGetShaderInfoLog ( shader, infoLen, &len, infoLog );    
            free ( infoLog );
        }
        glDeleteShader ( shader );
        return 0;
    }

    return shader;
}

void 
Shader::generateShaderInfo(GLuint shaderProgram)
{
	int attributeCount = 0;
	glGetProgramiv(shaderProgram,GL_ACTIVE_ATTRIBUTES, &attributeCount);
    m_ShaderAttributesNum = attributeCount;

	int maxAttributeLen = 0;
	glGetProgramiv(shaderProgram,GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeLen);

	if (attributeCount && maxAttributeLen)
	{
		char* nameBuffer = new char[maxAttributeLen];

		char* attributeBuffer = new char[sizeof(ShaderAttributeDef)*attributeCount];
		ShaderAttributeDef* attributeDef = m_ShaderAttributes = reinterpret_cast<ShaderAttributeDef*>(attributeBuffer);

		for (int i = 0; i < attributeCount; ++i)
		{
			GLint arraySize;
			GLenum valueType;
			glGetActiveAttrib(shaderProgram, i, maxAttributeLen, 0, &arraySize, &valueType, nameBuffer);
			size_t hashedName =  RTHASH(nameBuffer);
			GLint loc = glGetAttribLocation(shaderProgram,nameBuffer);

            E_Vertex_Attribute attributeType = getShaderVertexAttribute(nameBuffer);

			attributeDef->location = loc;
            attributeDef->attributeType = attributeType;

			attributeDef++;
		}

		delete[] nameBuffer;
	}


    int uniformCount = 0;
    glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &uniformCount);

    int maxUniformLen = 0;
    glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLen);

    if (uniformCount && maxUniformLen)
    {
        char* nameBuffer = new char[maxUniformLen];

		char* uniformBuffer = new char[sizeof(ShaderUniformDef)*uniformCount];
		ShaderUniformDef* uniformDef = reinterpret_cast<ShaderUniformDef*>(uniformBuffer);

        for(int i=0; i<uniformCount; ++i)
        {
            GLint arraySize;
            GLenum valueType;
            glGetActiveUniform(shaderProgram, i, maxUniformLen,0,&arraySize, &valueType, nameBuffer);
            GLint loc = glGetUniformLocation(shaderProgram,nameBuffer);
			size_t hashedName =  RTHASH(nameBuffer);
			uniformDef->arraySize = arraySize;
			uniformDef->valueType = valueType;
			uniformDef->location = loc;
			mShaderUniformsInfo.insert(std::pair<size_t, ShaderUniformDef*>(hashedName,uniformDef));
			uniformDef++;
        }
		delete []nameBuffer;
    }
}