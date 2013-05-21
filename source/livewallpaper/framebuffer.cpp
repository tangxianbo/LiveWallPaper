#include "framebuffer.h"
#include "texture2d.h"
#include <assert.h>

FrameBuffer::FrameBuffer(GLuint width,GLuint height)
			:m_width(width)
			,m_height(height)
			,m_frameBuffer(0)
			,m_depthBuffer(0)
			,m_targetTexture(0)
{

	glGenRenderbuffers(1,&m_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16,m_width, m_height);

	glGenTextures(1, &m_targetTexture);
	glBindTexture(GL_TEXTURE_2D,m_targetTexture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,m_width,m_height,0,GL_RGB,GL_UNSIGNED_BYTE,0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(1,&m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,m_targetTexture,0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

	GLuint uStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (uStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		assert(0);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1,&m_frameBuffer);
	glDeleteRenderbuffers(1,&m_depthBuffer);
	glDeleteTextures(1, &m_targetTexture);
}
