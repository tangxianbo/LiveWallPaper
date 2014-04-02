"#version 300 es																			\n\
precision highp float;																		\n\
																							\n\
in vec3 oldPos;																				\n\
in vec3 newPos;																				\n\
in vec3 ray;																				\n\
in vec3 normalColor;																		\n\
layout(location = 0) out vec4 outColor;														\n\
																							\n\
void main()																					\n\
{                                                                                           \n\
	float oldArea = length(dFdx(oldPos)) * length(dFdy(oldPos));							\n\
	float newArea = length(dFdx(newPos)) * length(dFdy(newPos));							\n\
	outColor = vec4(oldArea/newArea*0.2, 0.0, 0.0, 0.0);									\n\
	//outColor = vec4(oldArea/newArea,0.0, 0.0, 0.0);														\n\
	//outColor = vec4(1.0, 0.0, 0.0, 0.0);														\n\
}																							\n\
";