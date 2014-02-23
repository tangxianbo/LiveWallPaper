"															\n\
attribute vec3 position;									\n\
//attribute vec2 texcoord;									\n\
															\n\
uniform highp mat4 WVPMatrix;								\n\
															\n\
varying vec3 vPosition;										\n\
															\n\
void main()													\n\
{                                                           \n\
	gl_Position = WVPMatrix * vec4(position, 1.0);			\n\
//	vPosition = vec3(texcoord, 1.0);						\n\
}															\n\
";