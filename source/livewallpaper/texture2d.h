#pragma once
#include <GLES2/gl2.h>

class Texture2D
{
public:
	Texture2D(GLuint width, GLuint height, GLenum format, GLenum type);
	~Texture2D();

	void bind(GLuint index);
	void unbind(GLuint index);

	void swap(Texture2D* other);

	GLuint getId();

private:
	GLuint m_width;
	GLuint m_height;
	GLuint id;
};

inline GLuint 
Texture2D::getId()
{
	return id;
}