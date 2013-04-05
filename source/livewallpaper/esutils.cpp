#include "esutils.h"

void esLogMessage ( const char *formatStr, ... )
{
	va_list params;
	char buf[BUFSIZ];

	va_start ( params, formatStr );
	vsprintf_s ( buf, sizeof(buf),  formatStr, params );

	printf ( "%s", buf );

	va_end ( params );
}

GLuint LoadShader ( GLenum type, const char *shaderSrc )
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader ( type );

	if ( shader == 0 )
		return 0;

	// Load the shader source
	glShaderSource ( shader, 1, &shaderSrc, NULL );

	// Compile the shader
	glCompileShader ( shader );

	// Check the compile status
	glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

	if ( !compiled ) 
	{
		GLint infoLen = 0;

		glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = reinterpret_cast<char*>(malloc (sizeof(char) * infoLen ));

			glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
			esLogMessage ( "Error compiling shader:\n%s\n", infoLog );            

			free ( infoLog );
		}

		glDeleteShader ( shader );
		return 0;
	}

	return shader;

}

EGLBoolean CreateEGLContext(EGLNativeWindowType  hWnd,
							EGLDisplay* eglDisplay,
							EGLContext* eglContext,
							EGLSurface* eglSurface,
							EGLint attribList[])
{
	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLDisplay display;
	EGLContext context;
	EGLSurface surface;
	EGLConfig config;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

	// Get Display
	display = eglGetDisplay(GetDC(hWnd));
	if (display == EGL_NO_DISPLAY)
		return EGL_FALSE;

	if(!eglInitialize(display,&majorVersion,&minorVersion))
		return EGL_FALSE;

	if(!eglGetConfigs(display,NULL,0,&numConfigs))
		return EGL_FALSE;

	if(!eglChooseConfig(display,attribList,&config,1,&numConfigs))
		return EGL_FALSE;

	surface = eglCreateWindowSurface(display,config,(EGLNativeWindowType)hWnd,NULL);
	if(surface == EGL_NO_SURFACE)
		return EGL_FALSE;

	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );
	if ( context == EGL_NO_CONTEXT )
		return EGL_FALSE;

	if ( !eglMakeCurrent(display, surface, surface, context) )
		return EGL_FALSE;

	*eglDisplay = display;
	*eglSurface = surface;
	*eglContext = context;

	return EGL_TRUE;
}