"												\n\
precision lowp float;							\n\
uniform sampler2D water;						\n\
uniform vec3 light;								\n\
uniform vec3 eye;								\n\
												\n\
varying vec3 vPosition;							\n\
varying vec2 vCoord;							\n\
												\n\
float two2one(vec2 value)						\n\
{                                               \n\
	return dot(value, vec2(1.0, 1.0/255.0));    \n\
}												\n\
void main()										\n\
{                                               \n\
	vec4 info = texture2D(water, vCoord);		\n\
	float value = two2one(info.rg);				\n\
	gl_FragColor = vec4(value, 0.0, 0.0, 1.0);						\n\
}												\n\
";