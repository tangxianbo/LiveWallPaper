"															\n\
attribute vec3 position;									\n\
attribute vec2 texcoord;									\n\
															\n\
uniform sampler2D water;									\n\
uniform highp mat4 WVPMatrix;								\n\
															\n\
varying vec3 vPosition;										\n\
															\n\
void main()													\n\
{                                                           \n\
	vec4 info = texture2D(water,position.xy*0.5 + 0.5);		\n\
	vPosition = position.xzy;								\n\
	vPosition += info.r;									\n\
	gl_Position = WVPMatrix * vec4(vPosition, 1.0);			\n\
}															\n\
";