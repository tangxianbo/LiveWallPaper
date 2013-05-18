#pragma once
#include <GLES2/gl2.h>
#include "texture2d.h"

class Texture2D;
class FrameBuffer
{
public:
	FrameBuffer(GLuint width,GLuint height);
	~FrameBuffer();

	void SetColorAttachment(Texture2D* colorTexture);
	void Begin();
	void End();

private:
	GLuint m_width;
	GLuint m_height;

	GLuint m_frameBuffer;
	GLuint m_depthBuffer;
	GLuint m_targetTexture;
};

inline void 
FrameBuffer::SetColorAttachment(Texture2D* colorTexture)
{
	this->m_targetTexture = colorTexture->getId();
}