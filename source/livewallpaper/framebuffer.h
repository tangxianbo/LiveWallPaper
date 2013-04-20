#pragma once
#include <GLES2/gl2.h>

class FrameBuffer
{
public:
	FrameBuffer(GLuint width,GLuint height);
	~FrameBuffer();

	void SetColorAttachment(GLuint textureObject);
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
FrameBuffer::SetColorAttachment(GLuint textureObject)
{
	m_targetTexture = textureObject;
}