
"												\n\
attribute vec4 position;						\n\
attribute vec2 texcoord;						\n\
varying vec2 coord;								\n\
void main()										\n\
{												\n\
	coord = vec2(texcoord.x, 1.0-texcoord.y);	\n\
	gl_Position = position;						\n\
}												\n\
";