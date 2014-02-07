"												\n\
precision lowp float;							\n\
uniform sampler2D texture;						\n\
uniform vec2 delta;								\n\
varying vec2 coord;								\n\
void main()										\n\
{												\n\
	vec4 info = texture2D(texture, coord);		\n\
	vec2 dx = vec2(delta.x, 0.0);				\n\
	vec2 dy = vec2(0.0, delta.y);				\n\
	float average = (							\n\
		texture2D(texture, coord-dx).r +		\n\
		texture2D(texture, coord-dy).r +		\n\
		texture2D(texture, coord+dx).r +		\n\
		texture2D(texture, coord+dy).r			\n\
		)*0.25;									\n\
	info.g += (average - info.r)*2.0;			\n\
	info.g *= 0.995;							\n\
	info.r += info.g;							\n\
	gl_FragColor = info;						\n\
	//gl_FragColor = vec4(average, 0, 0, 1.0);	\n\
}												\n\
";