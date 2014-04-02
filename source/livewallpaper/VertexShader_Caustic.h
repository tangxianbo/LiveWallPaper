"#version 300 es																			\n\
																							\n\
const float IOR_AIR = 1.0;																	\n\
const float IOR_WATER = 1.333;																\n\
																							\n\
layout(location = 0) in vec3 position;														\n\
																							\n\
uniform vec3 light;																			\n\
uniform vec2 screenSize;																	\n\
uniform sampler2D water;																	\n\
																							\n\
out vec3 oldPos;																			\n\
out vec3 newPos;																			\n\
out vec3 ray;																				\n\
out vec3 normalColor;																							\n\
vec2 intersectCube(vec3 origin, vec3 ray, vec3 cubeMin, vec3 cubeMax)						\n\
{                                                                                           \n\
	vec3 tMin = (cubeMin - origin) / ray;													\n\
	vec3 tMax = (cubeMax - origin) / ray;													\n\
	vec3 t1 = min(tMin, tMax);																\n\
	vec3 t2 = max(tMin, tMax);																\n\
	float tNear = max(max(t1.x, t1.y), t1.z);												\n\
	float tFar = min(min(t2.x, t2.y), t2.z);												\n\
	return vec2(tNear, tFar);																\n\
}																							\n\
																							\n\
vec3 project(vec3 origin, vec3 ray, vec3 refractedLight)									\n\
{                                                                                           \n\
	//vec2 tcube = intersectCube(origin, ray, vec3(-1.0, -1.0, -1.0), vec3(1.0, 1.0, 2.0));   \n\
	//origin += ray* tcube.y;																	\n\
	float t = (-origin.z - 1.0)/refractedLight.z;											\n\
	return origin + refractedLight*t;														\n\
}																							\n\
																							\n\
void main()																					\n\
{                                                                                           \n\
	vec4 info = texture(water, position.xy);												\n\
	info.ba *= 0.5;																			\n\
	vec3 normal = vec3(info.b, sqrt(1.0 - dot(info.ba, info.ba)), info.a);					\n\
	vec3 refractedLight = refract(-light, vec3(0.0,0.0,1.0), IOR_AIR/IOR_WATER);			\n\
	normalColor = refractedLight;															\n\
	ray = refract(-light, normal, IOR_AIR/IOR_WATER);										\n\
	//vec3 vp = vec3((position.x-0.5)*screenSize.x, (position.y-0.5)*screenSize.y, 0.0);	\n\
	//vec3 vp = vec3(position.x, position.y, 0.0);									\n\
	vec3 vp = vec3((position.x-0.5)*2.0, (position.y-0.5)*2.0, 0.0);									\n\
	oldPos = project(vp, refractedLight, refractedLight);									\n\
	newPos = project(vp + vec3(0.0, 0.0, info.r), ray, refractedLight);						\n\
	gl_Position = vec4(0.75 * (newPos.xy + refractedLight.xy / refractedLight.z), 0.0, 1.0);\n\
	//gl_Position = vec4(position.x-0.5, position.y-0.5, 0.0, 1.0);									\n\
	//gl_Position = vec4(newPos, 1.0);														\n\
}																							\n\
";