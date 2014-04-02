"																							\n\
attribute vec3 position;																	\n\
																							\n\
uniform sampler2D water;																	\n\
uniform highp mat4 WVPMatrix;																\n\
uniform highp vec2 screenSize;																\n\
																							\n\
varying vec3 vPosition;																		\n\
varying vec2 vCoord;																		\n\
																							\n\
void main()																					\n\
{																							\n\
	vCoord = position.xy;																	\n\
	vec4 info = texture2D(water, position.xy);												\n\
	vPosition = vec3((position.x-0.5)*screenSize.x, (position.y-0.5)*screenSize.y, info.r);	\n\
	gl_Position = WVPMatrix * vec4(vPosition, 1.0);											\n\
}																							\n\
";