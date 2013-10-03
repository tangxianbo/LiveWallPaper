
"												\n\
attribute vec2 position;						\n\
varying vec2 coord;								\n\
void main()										\n\
{												\n\
	coord = position.xy;						\n\
	gl_Position = vec4(position.xy-0.5, 0,1.0); \n\
}												\n\
";