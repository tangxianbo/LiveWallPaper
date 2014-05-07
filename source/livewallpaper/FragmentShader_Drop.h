"																	\n\
precision highp float;												\n\
const float PI = 3.141592653589793;									\n\
uniform sampler2D texture;											\n\
uniform vec2 center;												\n\
uniform float radius;												\n\
uniform float strength;												\n\
uniform float scaleX;												\n\
varying vec2 coord;													\n\
																	\n\
float len(vec2 left, vec2 right, float scaleX)						\n\
{																	\n\
	float lenX = (right.x - left.x)*scaleX;							\n\
	float lenY = (right.y - left.y);								\n\
	float squareSum = lenX*lenX + lenY*lenY;						\n\
	return sqrt(squareSum);											\n\
}																	\n\
																	\n\
vec2 one2two(float value)											\n\
{                                                                   \n\
	vec2 ret = vec2(1.0, 255.0)*value;                              \n\
	ret = fract(ret);                                               \n\
	ret -= ret.yx* vec2(1.0/255.0, 0.0);                            \n\
	return ret;                                                     \n\
}	                                                                \n\
																	\n\
float two2one(vec2 value)											\n\
{                                                                   \n\
	return dot(value, vec2(1.0, 1.0/255.0));						\n\
}																	\n\
																	\n\
void main()															\n\
{																	\n\
	vec4 info = texture2D(texture, coord);							\n\
	vec2 adjustedCoord = vec2(coord.x, coord.y);					\n\
	float drop = max(0.0,1.0 - len(center, coord, scaleX)/radius);	\n\
	drop = 0.25 - cos(drop*PI)*0.25;									\n\
																		\n\
	//get value from texture, should +0.5f								\n\
	float oriValue = two2one(info.rg) - 0.5;						\n\
	oriValue = min(0.49999, oriValue + drop*strength);					\n\
																	\n\
	info.ba = info.rg;												\n\
	info.rg = one2two(oriValue + 0.5); //set value, should -0.5f	\n\
																	\n\
	gl_FragColor = info;											\n\
}																	\n\
";