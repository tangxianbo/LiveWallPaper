#pragma once

#include <assert.h>

typedef unsigned int	u32;
typedef signed int		s32;        
typedef float			f32;

#define JENNY_STATIC_ASSERT(x) static_assert(x, #x)
#define JENNY_ASSERT(x) assert(x)

#define isnan _isnan

enum class Math
{
	X = 0,
	Y,
	Z,
	W
};