#pragma once
#include <GLES2/gl2.h>

class Texture2D
{
public:
	Texture2D(GLuint width, GLuint height, GLenum format, GLenum type);
	~Texture2D();

	void bind(GLuint index);
	void unBind(GLuint index);

	void swap(Texture2D* other);

private:
	GLuint id;
};