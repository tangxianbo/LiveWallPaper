#pragma once
#include <GLES2/gl2.h>
#include "texture2d.h"

class Texture2D;
class FrameBuffer
{
public:
	FrameBuffer(GLuint width,GLuint height);
	~FrameBuffer();

	void bindColorTexture(GLuint slot);

	void Begin();
	void End();

private:
	GLuint m_width;
	GLuint m_height;

	GLuint m_frameBuffer;
	GLuint m_depthBuffer;
	GLuint m_targetTexture;
};

inline void FrameBuffer::Begin()
{
	glBindFramebuffer(GL_FRAMEBUFFER,m_frameBuffer);
}

inline void FrameBuffer::End()
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

inline void 
FrameBuffer::bindColorTexture(GLuint slot)
{
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, m_targetTexture);
}