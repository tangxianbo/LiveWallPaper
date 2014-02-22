#pragma once
#include <core/types.h>

enum class E_Vertex_Attribute
{
	EVA_POSITION = 0,

	EVA_TEXCOORD0,

	EVA_TEXCOORD1,

	EVA_TEXCOORD2,

	EVA_TEXCOORD3,

	EVA_TEXCOORD4,

	EVA_TEXCOORD5,

	EVA_TEXCOORD6,

	EVA_TEXCOORD7,

	EVA_TEXCOORD8,

	EVA_TEXCOORD9,

	EVA_TEXCOORD10,

	EVA_TEXCOORD11,

	EVA_TEXCOORD12,

	EVA_TEXCOORD13,

	EVA_TEXCOORD14,

	EVA_TEXCOORD15,

	EVA_NORMAL,

	EVA_COLOR0,

	EVA_COLOR1,

	EVA_TANGENT0,

	EVA_TANGENT1,

	EVA_TANGENT2,

	EVA_TANGENT3,

	EVA_BINORMAL0,

	EVA_BINORMAL1,

	EVA_BINORMAL2,

	EVA_BINORMAL3,

	EVA_SKIN_WEIGHTS,

	EVA_SKIN_INDICES,

	EVA_COUNT,

	EVA_FIRST = EVA_POSITION,

	EVA_TANGENT = EVA_TANGENT0,
	EVA_BINORMAL = EVA_BINORMAL0,

	EVA_UNKNOWN = u8(-1),
};

enum class E_VERTEX_ATTRIBUTE_BITS
{
	EVAB_POSITION = 1 << static_cast<int>(E_Vertex_Attribute::EVA_POSITION),

	EVAB_TEXCOORD0 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_TEXCOORD0),

	EVAB_TEXCOORD1 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_TEXCOORD1),

	EVAB_TEXCOORD2 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_TEXCOORD2),

	EVAB_TEXCOORD3 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_TEXCOORD3),

	EVAB_TEXCOORD4 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_TEXCOORD4),

	EVAB_TEXCOORD5 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_TEXCOORD5),

	EVAB_TEXCOORD6 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_TEXCOORD6),

	EVAB_TEXCOORD7 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_TEXCOORD7),

	EVAB_TEXCOORD8 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_TEXCOORD8),

	EVAB_TEXCOORD9 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_TEXCOORD9),

	EVAB_TEXCOORD10 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_TEXCOORD10),

	EVAB_TEXCOORD11 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_TEXCOORD11),

	EVAB_TEXCOORD12 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_TEXCOORD12),

	EVAB_TEXCOORD13 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_TEXCOORD13),

	EVAB_TEXCOORD14 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_TEXCOORD14),

	EVAB_TEXCOORD15 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_TEXCOORD15),

	EVAB_NORMAL = 1 << static_cast<int>(E_Vertex_Attribute::EVA_NORMAL),

	EVAB_COLOR0 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_COLOR0),

	EVAB_COLOR1 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_COLOR1),


	EVAB_TANGENT0 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_TANGENT0),

	EVAB_TANGENT1 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_TANGENT1),

	EVAB_TANGENT2 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_TANGENT2),

	EVAB_TANGENT3 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_TANGENT3),

	EVAB_BINORMAL0 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_BINORMAL0),

	EVAB_BINORMAL1 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_BINORMAL1),

	EVAB_BINORMAL2 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_BINORMAL2),

	EVAB_BINORMAL3 = 1 << static_cast<int>(E_Vertex_Attribute::EVA_BINORMAL3),

	EVAB_SKIN_WEIGHTS = 1 << static_cast<int>(E_Vertex_Attribute::EVA_SKIN_WEIGHTS),

	EVAB_SKIN_INDICES = 1 << static_cast<int>(E_Vertex_Attribute::EVA_SKIN_INDICES),

	EVAB_TEXCOORDS_MASK = (unsigned int)(~(EVAB_TEXCOORD0 - 1) & ((EVAB_TEXCOORD15 << 1) - 1)),

	EVAB_LAST_ATTRIBUTE = EVAB_SKIN_INDICES,

	EVAB_TANGENT = EVAB_TANGENT0,
	EVAB_BINORMAL = EVAB_BINORMAL0
};

enum class E_VERTEX_ATTRIBUTE_VALUE_TYPE
{
	//!
	EVAVT_BYTE,

	//!
	EVAVT_UBYTE,

	//!
	EVAVT_SHORT,

	//!
	EVAVT_USHORT,

	//!
	EVAVT_INT,

	//!
	EVAVT_UINT,

	//!
	EVAVT_FLOAT,

	EVAVT_COUNT,

	EVAVT_UNKNOWN = u8(-1)
};

E_Vertex_Attribute getShaderVertexAttribute(const char* name);