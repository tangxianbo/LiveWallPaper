#include "texture2d.h"
#include <stdlib.h>

Texture2D::Texture2D(GLuint width, GLuint height, GLenum format, GLenum type)
					:id(0)
{
	glGenTextures(1,&id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_FLOAT,NULL);
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

void Texture2D::unBind(GLuint index)
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