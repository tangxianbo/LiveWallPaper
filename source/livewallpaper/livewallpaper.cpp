#include "livewallpaper.h"
#include "esutils.h"
#include "water.h"

#include <math/matrix4.h>

using namespace jenny;
jenny::matrix4 g_viewMatrix;
jenny::matrix4 g_projectMatrix;
jenny::matrix4 g_viewProjectMatrix;

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

	//initialize matrixs
	//view matrix
	vector3df pos(0.0f, 0.0f, 6.0f);
	vector3df target(0.0f, 0.0f, 0.0f);
	vector3df up(0.0f, 1.0f, 0.0f);
	g_viewMatrix = jenny::buildCameraLookAtMatrix(pos, target, up);

	//projection matrix
	float fovAngleY = 70.0f * PI / 180.0f;
	float aspectRatio = m_width / m_height;
	g_projectMatrix = buildProjectionMatrixPerspectiveFov(fovAngleY, aspectRatio, 0.1f, 300.0f);

	//view projecton matrix
	g_viewProjectMatrix.setbyproduct_nocheck(g_projectMatrix, g_viewMatrix);

	m_water = new Water(m_width,m_height,200.0f);
	m_water->Init();
}


void LiveWallPaper::Update()
{
	//update touches
	while (m_touchQueue.size() > 0)
	{
		TouchPos touch = m_touchQueue.front();
		m_touchQueue.pop();
		m_water->onTouch(touch.X, touch.Y);
	}

	m_water->Update();
}

void LiveWallPaper::Render()
{
	glViewport ( 0, 0, m_width, m_height);
	glClear ( GL_COLOR_BUFFER_BIT );

	//render water
	m_water->Render();

	eglSwapBuffers ( m_eglDisplay, m_eglSurface);
}

void LiveWallPaper::OnTouch(int x, int y)
{
	TouchPos touchPos(x,y);
	m_touchQueue.push(touchPos);
}