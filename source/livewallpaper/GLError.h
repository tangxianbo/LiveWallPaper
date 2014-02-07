#pragma once
#include <GLES3/gl3.h>

void _check_gl_error(const char* file, int line);

#define  check_gl_error() _check_gl_error(__FILE__, __LINE__);
