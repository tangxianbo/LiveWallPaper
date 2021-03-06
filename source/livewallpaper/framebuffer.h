#pragma once
#include <GLES3/gl3.h>
#include "texture2d.h"


enum 
{
	EFBT_TEXTURE_RGB8 = 1 << 1,
	EFBT_TEXTURE_RGBA8 = 1 << 2,
	EFBT_TEXTURE_DEPTH = 1 << 3,
	EFBT_TEXTURE_WHITE = 1 << 4,

	EFBT_TEXTURE = EFBT_TEXTURE_RGB8 | EFBT_TEXTURE_RGBA8,
};

class Texture2D;
class FrameBuffer
{
public:
	FrameBuffer(GLuint width,GLuint height, unsigned int flags);
	~FrameBuffer();

	void Begin();
	void End();
	bool Swap(FrameBuffer* other);
	GLuint GetColorTexture();
	GLuint GetDepthTexture();
	GLuint GetWidth();
	GLuint GetHeight();

private:
	GLuint m_width;
	GLuint m_height;

	GLuint m_frameBuffer;
	GLuint m_depthBuffer;
	GLuint m_targetTexture;

	unsigned int m_flags;
};

inline void FrameBuffer::Begin()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_targetTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthBuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_targetTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER,m_frameBuffer);
}

inline void FrameBuffer::End()
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

inline GLuint FrameBuffer::GetColorTexture()
{
	return m_targetTexture;
}

inline GLuint FrameBuffer::GetDepthTexture()
{
	return m_depthBuffer;
}

inline GLuint FrameBuffer::GetWidth()
{
	return m_width;
}

inline GLuint FrameBuffer::GetHeight()
{
	return m_height;
}