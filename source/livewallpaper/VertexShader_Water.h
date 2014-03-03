"																						\n\
attribute vec3 position;																\n\
																						\n\
uniform highp sampler2D water;															\n\
uniform highp mat4 WVPMatrix;															\n\
uniform highp vec2 screenSize;															\n\
																						\n\
varying vec3 vPosition;																	\n\
																						\n\
void main()																				\n\
{																						\n\
	vec4 info = texture2D(water, position.xy);											\n\
	vec2 adjPos = vec2((position.x-0.5)*screenSize.x, (position.y-0.5)*screenSize.y);	\n\
	//adjPos.x += info.r;																	\n\
	gl_Position = WVPMatrix * vec4(adjPos, info.r, 1.0);									\n\
	//vPosition = vec3(texcoord, 1.0);													\n\
}																						\n\
";