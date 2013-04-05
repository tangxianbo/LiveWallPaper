#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "base/singleton.h"

class Water;
class LiveWallPaper:public Singleton<LiveWallPaper>
{
	friend Singleton<LiveWallPaper>;

protected:
	LiveWallPaper();
	~LiveWallPaper();

public:
	void Init(int width, int height, HWND hwnd);
	void Update();

private:
	HWND		m_hWnd;

	GLuint		m_width;
	GLuint		m_height;

	EGLDisplay	m_eglDisplay;
	EGLContext	m_eglContext;
	EGLSurface	m_eglSurface;

	Water*		m_water;
};