"												\n\
precision highp float;							\n\
uniform sampler2D texture;						\n\
uniform vec2 delta;								\n\
varying vec2 coord;								\n\
												\n\
vec2 one2two(float value)						\n\
{                                               \n\
	vec2 ret = vec2(1.0, 255.0)*value;           \n\
	ret = fract(ret);							\n\
	ret -= ret.yx* vec2(1.0/255.0, 0.0);			\n\
	return ret;										\n\
}															\n\
													\n\
float two2one(vec2 value)								\n\
{                                                      \n\
	return dot(value, vec2(1.0, 1.0/255.0));			\n\
}														\n\
														\n\
void main()												\n\
{														\n\
	vec4 info = texture2D(texture, coord);				\n\
	vec2 dx = vec2(delta.x, 0.0);						\n\
	vec2 dy = vec2(0.0, delta.y);							\n\
	float average = (										\n\
		two2one(texture2D(texture, coord-dx).rg) +				\n\
		two2one(texture2D(texture, coord-dy).rg) +				\n\
		two2one(texture2D(texture, coord+dx).rg) +					\n\
		two2one(texture2D(texture, coord+dy).rg) - 2.0			\n\
		)*0.5;													\n\
																	\n\
	//average = clamp(average - two2one(info.ba) + 0.5, -0.5, 0.5);		\n\
	average = average - two2one(info.ba) + 0.5;   \n\
	average *= 0.96;													\n\
																	\n\
	info.ba = info.rg;											\n\
	info.rg = one2two(average + 0.5);							\n\
																\n\
	gl_FragColor = info;											\n\
}																	\n\
";																	