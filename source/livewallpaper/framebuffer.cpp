#include "framebuffer.h"
#include "texture2d.h"
#include <assert.h>
#include <algorithm>  
#include <vector>

FrameBuffer::FrameBuffer(GLuint width,GLuint height,unsigned int flags)
			:m_width(width)
			,m_height(height)
			,m_frameBuffer(0)
			,m_depthBuffer(0)
			,m_targetTexture(0)
			,m_flags(flags)
{	
	glGenFramebuffers(1,&m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

	if(m_flags & EFBT_TEXTURE)
	{
		GLuint textureFormat = (m_flags & EFBT_TEXTURE_RGB8) ? GL_RGB : GL_RGBA;
		int bytesPerPixel = (m_flags & EFBT_TEXTURE_RGB8) ? 3 : 4;

		glGenTextures(1, &m_targetTexture);
		glBindTexture(GL_TEXTURE_2D,m_targetTexture);
		if(m_flags & EFBT_TEXTURE_WHITE)
		{
			std::vector<GLubyte> textureData(m_width*m_height*bytesPerPixel,125);
			GLubyte* pData = new GLubyte[m_width*m_height*bytesPerPixel];
			for (int i=0; i< m_width*m_height*bytesPerPixel; ++i)
			{
				pData[i] = 255;
			}
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D,0,textureFormat,m_width,m_height,0,textureFormat,GL_UNSIGNED_BYTE,pData);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D,0,textureFormat,m_width,m_height,0,textureFormat,GL_UNSIGNED_BYTE,0);
		}

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,m_targetTexture,0);
	}

	
	if(m_flags & EFBT_TEXTURE_DEPTH)
	{
		glGenRenderbuffers(1,&m_depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16,m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
	}

	GLuint uStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (uStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		assert(0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1,&m_frameBuffer);
	glDeleteRenderbuffers(1,&m_depthBuffer);
	glDeleteTextures(1, &m_targetTexture);
}

bool FrameBuffer::Swap(FrameBuffer* other)
{
	if(other == nullptr
		|| this->m_flags != other->m_flags)
		return false;

	std::swap(this->m_frameBuffer,other->m_frameBuffer);
	std::swap(this->m_depthBuffer, other->m_depthBuffer);
	std::swap(this->m_targetTexture,other->m_targetTexture);

	return true;
}
