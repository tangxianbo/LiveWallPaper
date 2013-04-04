#include "livewallpaper.h"
#include "esutils.h"


LiveWallPaper::LiveWallPaper():	m_hWnd(nullptr)
								,m_programObject(0)
{

}


LiveWallPaper::~LiveWallPaper()
{

}


void LiveWallPaper::Init(int width, int height, HWND hwnd)
{
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

	_initShader();
}


void LiveWallPaper::_initShader()
{
	const char vShaderStr[] =  
		"attribute vec4 vPosition;    \n"
		"void main()                  \n"
		"{                            \n"
		"   gl_Position = vPosition;  \n"
		"}                            \n";

	const char fShaderStr[] =  
		"precision mediump float;\n"\
		"void main()                                  \n"
		"{                                            \n"
		"  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
		"}                                            \n";

	GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
	GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER,fShaderStr);

	m_programObject = glCreateProgram();
	if(0 == m_programObject)
		return;

	glAttachShader(m_programObject,vertexShader);
	glAttachShader(m_programObject,fragmentShader);

	glBindAttribLocation(m_programObject,0,"vPosition");

	glLinkProgram(m_programObject);

	GLint linked;
	glGetProgramiv(m_programObject,GL_LINK_STATUS,&linked);
	if(!linked)
	{
		glDeleteProgram(m_programObject);
		m_programObject = 0;
		return;
	}

}

void LiveWallPaper::_initTexture()
{

}