#pragma once

#include <assert.h>

typedef unsigned char	u8;         
typedef signed char		s8;         
typedef char			c8;         

typedef unsigned short	u16;        
typedef signed short	s16;        

typedef unsigned int	u32;        
typedef signed int		s32;        
typedef float			f32;
typedef double			f64;

#define JENNY_STATIC_ASSERT(x) static_assert(x, #x)
#define JENNY_ASSERT(x) assert(x)
#define JENNY_DEBUG_BREAK_IF(x) assert(!x)

#define isnan _isnan