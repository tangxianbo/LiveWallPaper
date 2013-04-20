#include "framebuffer.h"

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
	
}

void FrameBuffer::End()
{

}