#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <queue>
#include "base/singleton.h"

struct TouchPos
{
	TouchPos(int x, int y):X(x),Y(y){}
	int X;
	int Y;
};

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
	void Render();
	void OnTouch(int x, int y);

private:
	HWND		m_hWnd;

	GLuint		m_width;
	GLuint		m_height;

	EGLDisplay	m_eglDisplay;
	EGLContext	m_eglContext;
	EGLSurface	m_eglSurface;

	Water*		m_water;

	std::queue<TouchPos>	m_touchQueue;
};