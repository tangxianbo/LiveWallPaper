#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "base/singleton.h"

class LiveWallPaper:public Singleton<LiveWallPaper>
{
	friend Singleton<LiveWallPaper>;

protected:
	LiveWallPaper();
	~LiveWallPaper();

public:
	void Init(int width, int height, HWND hwnd);

private:
	void _initShader();
	void _initTexture();


private:
	HWND		m_hWnd;
	GLuint		m_programObject;
	EGLDisplay	m_eglDisplay;
	EGLContext	m_eglContext;
	EGLSurface	m_eglSurface;

};