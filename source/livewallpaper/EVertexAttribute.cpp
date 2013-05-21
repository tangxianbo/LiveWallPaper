#include "EVertexAttribute.h"
#include <map>
#include <string>
#include "base\string_hash.h"

E_Vertex_Attribute getShaderVertexAttribute(const char* name)
{
    typedef std::map<std::string, E_Vertex_Attribute> VertexAttributeMap;

#if defined(_MSC_VER)
    static std::map<std::string, E_Vertex_Attribute> attributeMap;
    if (0 == attributeMap.size())
    {
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "vertex",           E_Vertex_Attribute::EVA_POSITION));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "pos",		        E_Vertex_Attribute::EVA_POSITION));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "position",		    E_Vertex_Attribute::EVA_POSITION));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "vertices",		    E_Vertex_Attribute::EVA_POSITION));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "normal",		    E_Vertex_Attribute::EVA_NORMAL));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "normals",		    E_Vertex_Attribute::EVA_NORMAL));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "tangent",		    E_Vertex_Attribute::EVA_TANGENT0));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "tangents",		    E_Vertex_Attribute::EVA_TANGENT0));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "tangent0",		    E_Vertex_Attribute::EVA_TANGENT0));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "tangent1",		    E_Vertex_Attribute::EVA_TANGENT1));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "tangent2",		    E_Vertex_Attribute::EVA_TANGENT2));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "tangent3",		    E_Vertex_Attribute::EVA_TANGENT3));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "binormal",		    E_Vertex_Attribute::EVA_BINORMAL0));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "binormals",		E_Vertex_Attribute::EVA_BINORMAL0));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "binormal0",		E_Vertex_Attribute::EVA_BINORMAL0));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "binormal1",		E_Vertex_Attribute::EVA_BINORMAL1));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "binormal2",		E_Vertex_Attribute::EVA_BINORMAL2));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "binormal3",		E_Vertex_Attribute::EVA_BINORMAL3));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "diffuse",		    E_Vertex_Attribute::EVA_COLOR0));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "color",			E_Vertex_Attribute::EVA_COLOR0));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "color0",			E_Vertex_Attribute::EVA_COLOR0));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "color1",			E_Vertex_Attribute::EVA_COLOR1));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "secondarycolor",	E_Vertex_Attribute::EVA_COLOR1));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "alternatecolor",	E_Vertex_Attribute::EVA_COLOR1));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "coord",			E_Vertex_Attribute::EVA_TEXCOORD0));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "coord0",			E_Vertex_Attribute::EVA_TEXCOORD0));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "coord1",			E_Vertex_Attribute::EVA_TEXCOORD1));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "coord2",			E_Vertex_Attribute::EVA_TEXCOORD2));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "coord3",			E_Vertex_Attribute::EVA_TEXCOORD3));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "coord4",           E_Vertex_Attribute::EVA_TEXCOORD4));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "coord5",           E_Vertex_Attribute::EVA_TEXCOORD5));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "coord6",           E_Vertex_Attribute::EVA_TEXCOORD6));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "coord7",           E_Vertex_Attribute::EVA_TEXCOORD7));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "texcoord",		    E_Vertex_Attribute::EVA_TEXCOORD0));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "texcoord0",		E_Vertex_Attribute::EVA_TEXCOORD0));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "texcoord1",		E_Vertex_Attribute::EVA_TEXCOORD1));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "texcoord2",		E_Vertex_Attribute::EVA_TEXCOORD2));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "texcoord3",		E_Vertex_Attribute::EVA_TEXCOORD3));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "texcoord4",		E_Vertex_Attribute::EVA_TEXCOORD4));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "texcoord5",		E_Vertex_Attribute::EVA_TEXCOORD5));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "texcoord6",		E_Vertex_Attribute::EVA_TEXCOORD6));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "texcoord7",		E_Vertex_Attribute::EVA_TEXCOORD7));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "skinweights",      E_Vertex_Attribute::EVA_SKIN_WEIGHTS));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "skinweight",       E_Vertex_Attribute::EVA_SKIN_WEIGHTS));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "skinindices",      E_Vertex_Attribute::EVA_SKIN_INDICES));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "skinindex",        E_Vertex_Attribute::EVA_SKIN_INDICES));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "weights",          E_Vertex_Attribute::EVA_SKIN_WEIGHTS));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "bones",            E_Vertex_Attribute::EVA_SKIN_INDICES));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "blendweight",      E_Vertex_Attribute::EVA_SKIN_WEIGHTS));
        attributeMap.insert(std::pair<std::string, E_Vertex_Attribute>( "blendindices",     E_Vertex_Attribute::EVA_SKIN_INDICES));
    }
#else
    static const std::map<std::string, E_Vertex_Attribute> attributeMap = 
    {
        { "vertex",			E_Vertex_Attribute::EVA_POSITION},
        { "pos",		    E_Vertex_Attribute::EVA_POSITION},
        { "position",		E_Vertex_Attribute::EVA_POSITION},
        { "vertices",		E_Vertex_Attribute::EVA_POSITION},
        { "normal",			E_Vertex_Attribute::EVA_NORMAL},
        { "normals",		E_Vertex_Attribute::EVA_NORMAL},
        { "tangent",		E_Vertex_Attribute::EVA_TANGENT0},
        { "tangents",		E_Vertex_Attribute::EVA_TANGENT0},
        { "tangent0",		E_Vertex_Attribute::EVA_TANGENT0},
        { "tangent1",		E_Vertex_Attribute::EVA_TANGENT1},
        { "tangent2",		E_Vertex_Attribute::EVA_TANGENT2},
        { "tangent3",		E_Vertex_Attribute::EVA_TANGENT3},
        { "binormal",		E_Vertex_Attribute::EVA_BINORMAL0},
        { "binormals",		E_Vertex_Attribute::EVA_BINORMAL0},
        { "binormal0",		E_Vertex_Attribute::EVA_BINORMAL0},
        { "binormal1",		E_Vertex_Attribute::EVA_BINORMAL1},
        { "binormal2",		E_Vertex_Attribute::EVA_BINORMAL2},
        { "binormal3",		E_Vertex_Attribute::EVA_BINORMAL3},
        { "diffuse",		E_Vertex_Attribute::EVA_COLOR0},
        { "color",			E_Vertex_Attribute::EVA_COLOR0},
        { "color0",			E_Vertex_Attribute::EVA_COLOR0},
        { "color1",			E_Vertex_Attribute::EVA_COLOR1},
        { "secondarycolor",	E_Vertex_Attribute::EVA_COLOR1},
        { "alternatecolor",	E_Vertex_Attribute::EVA_COLOR1},
        { "coord",			E_Vertex_Attribute::EVA_TEXCOORD0},
        { "coord0",			E_Vertex_Attribute::EVA_TEXCOORD0},
        { "coord1",			E_Vertex_Attribute::EVA_TEXCOORD1},
        { "coord2",			E_Vertex_Attribute::EVA_TEXCOORD2},
        { "coord3",			E_Vertex_Attribute::EVA_TEXCOORD3},
        { "coord4",         E_Vertex_Attribute::EVA_TEXCOORD4},
        { "coord5",         E_Vertex_Attribute::EVA_TEXCOORD5},
        { "coord6",         E_Vertex_Attribute::EVA_TEXCOORD6},
        { "coord7",         E_Vertex_Attribute::EVA_TEXCOORD7},
        { "texcoord",		E_Vertex_Attribute::EVA_TEXCOORD0},
        { "texcoord0",		E_Vertex_Attribute::EVA_TEXCOORD0},
        { "texcoord1",		E_Vertex_Attribute::EVA_TEXCOORD1},
        { "texcoord2",		E_Vertex_Attribute::EVA_TEXCOORD2},
        { "texcoord3",		E_Vertex_Attribute::EVA_TEXCOORD3},
        { "texcoord4",		E_Vertex_Attribute::EVA_TEXCOORD4},
        { "texcoord5",		E_Vertex_Attribute::EVA_TEXCOORD5},
        { "texcoord6",		E_Vertex_Attribute::EVA_TEXCOORD6},
        { "texcoord7",		E_Vertex_Attribute::EVA_TEXCOORD7},
        { "skinweights",    E_Vertex_Attribute::EVA_SKIN_WEIGHTS},
        { "skinweight",     E_Vertex_Attribute::EVA_SKIN_WEIGHTS},
        { "skinindices",    E_Vertex_Attribute::EVA_SKIN_INDICES},
        { "skinindex",      E_Vertex_Attribute::EVA_SKIN_INDICES},
        { "weights",        E_Vertex_Attribute::EVA_SKIN_WEIGHTS},
        { "bones",          E_Vertex_Attribute::EVA_SKIN_INDICES},
        { "blendweight",    E_Vertex_Attribute::EVA_SKIN_WEIGHTS},
        { "blendindices",   E_Vertex_Attribute::EVA_SKIN_INDICES}
    };
#endif

    VertexAttributeMap::const_iterator it = attributeMap.find(name);
    if (it == attributeMap.end())
    {
        return E_Vertex_Attribute::EVA_UNKNOWN;
    }

    return it->second;
}