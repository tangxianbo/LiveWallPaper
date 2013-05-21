#include "texture2d.h"
#include <stdlib.h>

Texture2D::Texture2D(GLuint width, GLuint height, GLenum format, GLenum type)
					:m_width(width)
					,m_height(height)
					,id(0)
{
	glGenTextures(1,&id);
	glBindTexture(GL_TEXTURE_2D, id);

	////test
	//float* data = new float[width*height*4];
	//for (int i=0; i<width*height; ++i)
	//{
	//	*data++ = 255.0f;
	//	*data++ = 255.0f;
	//	*data++ = 1.0f;
	//	*data++ = 1.0f;
	//}
	////test end

	glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,type,0);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1,&id);
}

void Texture2D::bind(GLuint index)
{
	glActiveTexture(GL_TEXTURE0+(index||0));
	glBindTexture(GL_TEXTURE_2D,this->id);
}

void Texture2D::unbind(GLuint index)
{
	glActiveTexture(GL_TEXTURE0 + (index||0));
	glBindTexture(GL_TEXTURE_2D,NULL);
}

void Texture2D::swap(Texture2D* other)
{
	GLuint temp = other->id;
	this->id = other->id;
	other->id = temp;
}