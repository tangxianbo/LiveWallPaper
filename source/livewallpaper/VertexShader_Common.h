
"												\n\
attribute vec4 position;						\n\
attribute vec2 texcoord;						\n\
varying vec2 coord;								\n\
void main()										\n\
{												\n\
	coord = texcoord;							\n\
	gl_Position = position;						\n\
}												\n\
";