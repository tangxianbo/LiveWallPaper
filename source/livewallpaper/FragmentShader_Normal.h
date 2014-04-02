"																									\n\
precision highp float;																				\n\
uniform sampler2D texture;																			\n\
uniform vec2 delta;																					\n\
varying vec2 coord;																					\n\
void main(){                                                                                        \n\
	/*get vertex info */																			\n\
	vec4 info = texture2D(texture, coord);															\n\
																									\n\
	/* update the normal */																			\n\
	//vec3 dx = vec3(delta.x, texture2D(texture, vec2(coord.x + delta.x, coord.y)).r - info.r, 0.0);\n\
	//vec3 dy = vec3(0.0, texture2D(texture, vec2(coord.x, coord.y + delta.y)).r - info.r, delta.y);\n\
	vec3 dx = vec3(delta.x, 0.0, texture2D(texture, vec2(coord.x + delta.x, coord.y)).r - info.r);	\n\
	vec3 dy = vec3(0.0, delta.y, texture2D(texture, vec2(coord.x, coord.y + delta.y)).r - info.r);	\n\
	info.ba = normalize(cross(dx, dy)).xz;															\n\
	//vec3 normal = 	normalize(cross(dx, dy));													\n\
	//info.ba = normal.zx;																			\n\
	gl_FragColor = info;																			\n\
}																									\n\
";