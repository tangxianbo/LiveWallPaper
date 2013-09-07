
"																	\n\
precision lowp float;												\n\
const float PI = 3.141592653589793;									\n\
uniform sampler2D texture;											\n\
uniform vec2 center;												\n\
uniform float radius;												\n\
uniform float strength;												\n\
varying vec2 coord;													\n\
void main()															\n\
{                                                                   \n\
	vec4 info = texture2D(texture, coord);                          \n\
	float drop = max(0.0,1.0 - length(center+0.5-coord)/radius);    \n\
	drop = 0.5 - cos(drop*PI)*0.5;                                  \n\
	info.r += drop*strength;                                        \n\
	                                                                \n\
	gl_FragColor = info;                                            \n\
}																	\n\
";