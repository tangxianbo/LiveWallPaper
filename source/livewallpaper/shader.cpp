#include "shader.h"
#include "base/string_hash.h"


Shader::Shader(const char* verStr, const char* fragStr):mShaderProgram(0)
{
   GLuint vertShader = this->LoadShader(GL_VERTEX_SHADER,verStr);
   GLuint fragShader = this->LoadShader(GL_FRAGMENT_SHADER, fragStr);

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
			   GetShaderUniforms(mShaderProgram);
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
	delete[] mShaderUniforms;
}


GLuint 
Shader::LoadShader ( GLenum type, const char* shaderStr)
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
        glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
        if ( infoLen > 1 )
        {
            char* infoLog = reinterpret_cast<char*>(malloc (sizeof(char) * infoLen ));
            glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );      
            free ( infoLog );
        }
        glDeleteShader ( shader );
        return 0;
    }

    return shader;
}

void 
Shader::GetShaderUniforms(GLuint shaderProgram)
{
    int uniformCount = 0;
    glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &uniformCount);

    int maxUniformLen = 0;
    glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLen);

    if (uniformCount && maxUniformLen)
    {
        char* nameBuffer = new char[maxUniformLen+1];
        nameBuffer[maxUniformLen] = 0;

		char* uniformBuffer = new char[sizeof(ShaderUniformDef)*uniformCount];
		ShaderUniformDef* uniformDef = mShaderUniforms = reinterpret_cast<ShaderUniformDef*>(uniformBuffer);

        for(int i=0; i<uniformCount; ++i)
        {
            GLint arraySize;
            GLenum valueType;
            glGetActiveUniform(shaderProgram, i, maxUniformLen,0,&arraySize, &valueType, nameBuffer);
            GLint loc = glGetUniformLocation(shaderProgram,nameBuffer);
			uniformDef->hashedName =  RTHASH(nameBuffer);
			uniformDef->arraySize = arraySize;
			uniformDef->valueType = valueType;
			uniformDef->location = loc;
			uniformDef++;
        }
		delete []nameBuffer;
    }
}