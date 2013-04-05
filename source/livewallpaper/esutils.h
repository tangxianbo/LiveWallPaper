#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

/// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_RGB           0
/// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_ALPHA         1 
/// esCreateWindow flag - depth buffer
#define ES_WINDOW_DEPTH         2 
/// esCreateWindow flag - stencil buffer
#define ES_WINDOW_STENCIL       4
/// esCreateWindow flat - multi-sample buffer
#define ES_WINDOW_MULTISAMPLE   8

void esLogMessage ( const char *formatStr, ... );

GLuint LoadShader ( GLenum type, const char *shaderSrc );

EGLBoolean CreateEGLContext(EGLNativeWindowType  hWnd,
							EGLDisplay* eglDisplay,
							EGLContext* eglContext,
							EGLSurface* eglSurface,
							EGLint attribList[]);