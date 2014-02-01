"													\n\
attribute vec3 position;							\n\
attribute vec2 texcoord;							\n\
varying vec2 v_texCoord;							\n\
void main()											\n\
{													\n\
	gl_Position = vec4(position.xyz, 1.0);			\n\
	v_texCoord = vec2(texcoord.x, 1.0-texcoord.y);	\n\
}													\n\
";