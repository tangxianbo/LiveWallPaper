"																	\n\
precision lowp float;												\n\
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
void main()															\n\
{																	\n\
	vec4 info = texture2D(texture, coord);							\n\
	vec2 adjustedCoord = vec2(coord.x, coord.y);					\n\
	float drop = max(0.0,1.0 - len(center, coord, scaleX)/radius);	\n\
	drop = 0.5 - cos(drop*PI)*0.5;									\n\
	info.r += drop*strength;										\n\
	gl_FragColor = info;											\n\
}																	\n\
";