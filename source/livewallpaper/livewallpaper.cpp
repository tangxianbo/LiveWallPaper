#include "livewallpaper.h"
#include "esutils.h"
#include "water.h"


LiveWallPaper::LiveWallPaper():	m_hWnd(nullptr)
								,m_width(0)
								,m_height(0)
								,m_water(NULL)

{

}


LiveWallPaper::~LiveWallPaper()
{

}


void LiveWallPaper::Init(int width, int height, HWND hwnd)
{
	m_width = width;
	m_height = height;
	m_hWnd = hwnd;

	GLuint flags = ES_WINDOW_RGB;
	EGLint attribList[] =
	{
		EGL_RED_SIZE,       5,
		EGL_GREEN_SIZE,     6,
		EGL_BLUE_SIZE,      5,
		EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
		EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
		EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
		EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
		EGL_NONE
	};
	CreateEGLContext(m_hWnd,&m_eglDisplay,&m_eglContext,&m_eglSurface,attribList);


	m_water = new Water(m_width,m_height,200.0f);
	m_water->Init();
}


void LiveWallPaper::Update()
{
	glViewport ( 0, 0, m_width, m_height);
	glClear ( GL_COLOR_BUFFER_BIT );

	m_water->Render();

	eglSwapBuffers ( m_eglDisplay, m_eglSurface);
}

void LiveWallPaper::OnTouch(int x, int y)
{
	m_water->Touch(x,y);
}