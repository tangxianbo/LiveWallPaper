#include "framebuffer.h"
#include "texture2d.h"

FrameBuffer::FrameBuffer(GLuint width,GLuint height)
			:m_width(width)
			,m_height(height)
			,m_frameBuffer(0)
			,m_targetTexture(0)
{
	glGenFramebuffers(1,&m_frameBuffer);
	glGenRenderbuffers(1,&m_depthBuffer);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1,&m_frameBuffer);
	glDeleteRenderbuffers(1,&m_depthBuffer);
}


void FrameBuffer::Begin()
{
	glBindFramebuffer(GL_FRAMEBUFFER,m_frameBuffer);

	//attention, this may be placed in init.
	glBindRenderbuffer(GL_RENDERBUFFER,m_depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,m_width,m_height);

	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_targetTexture,0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,m_depthBuffer);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status == GL_FRAMEBUFFER_COMPLETE)
	{
		glViewport(0,0,m_width,m_height);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	}
}

void FrameBuffer::End()
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glBindRenderbuffer(GL_RENDERBUFFER,0);
}