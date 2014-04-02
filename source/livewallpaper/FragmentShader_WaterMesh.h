"																			\n\
precision lowp float;														\n\
uniform sampler2D water;													\n\
uniform vec3 light;															\n\
uniform vec3 eye;															\n\
																			\n\
varying vec3 vPosition;														\n\
varying vec2 vCoord;														\n\
																			\n\
void main()																	\n\
{																			\n\
	vec3 ambientColor = vec3(0.2, 0.2, 0.2);								\n\
	vec3 lightColor = vec3(0.7, 0.7, 0.7);									\n\
																			\n\
	vec4 info = texture2D(water, vCoord);									\n\
	//info.ba *= 0.5;															\n\
	vec3 normal = vec3(info.b, sqrt(1.0 - dot(info.ba, info.ba)), info.a);	\n\
	float diffuseTerm = max(0.0, dot(-light, normal));						\n\
																			\n\
	vec3 finalColor = ambientColor + lightColor*diffuseTerm;				\n\
																			\n\
	gl_FragColor = vec4(finalColor, 1.0);								\n\
}																			\n\
";