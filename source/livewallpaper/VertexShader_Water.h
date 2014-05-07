"																							\n\
attribute vec3 position;																	\n\
																							\n\
uniform sampler2D water;																	\n\
uniform highp mat4 WVPMatrix;																\n\
uniform highp vec2 screenSize;																\n\
uniform highp vec2 delta;																	\n\
																							\n\
varying vec3 vPosition;																		\n\
varying vec2 vCoord;																		\n\
																							\n\
void main()																					\n\
{																							\n\
	vec4 info = texture2D(water, position.xy);												\n\
	float dx = texture2D(water, vec2(position.x + delta.x, position.y)).r - info.r;			\n\
	float dy = texture2D(water, vec2(position.x, position.y + delta.y)).r - info.r;			\n\
	vCoord = vec2(position.x +dx, position.y + dy);											\n\
	vPosition = vec3((position.x-0.5)*screenSize.x, (position.y-0.5)*screenSize.y, info.r);	\n\
	gl_Position = WVPMatrix * vec4(vPosition, 1.0);											\n\
}																							\n\
";