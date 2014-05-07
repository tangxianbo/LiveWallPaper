"												\n\
precision lowp float;							\n\
uniform sampler2D base;							\n\
uniform vec3 light;								\n\
uniform vec3 eye;								\n\
												\n\
varying vec3 vPosition;							\n\
varying vec2 vCoord;							\n\
												\n\
void main()										\n\
{                                               \n\
	vec4 info = texture2D(base, vCoord);		\n\
	gl_FragColor = info;						\n\
}												\n\
";