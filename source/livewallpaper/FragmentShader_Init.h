"																	\n\
																\n\
precision highp float;												\n\
varying vec2 coord;														\n\
\n\
 vec2 one2two(float value)\n\
{\n\
	vec2 ret = vec2(1.0, 255.0)*value;\n\
	ret = fract(ret);\n\
	ret -= ret.yx* vec2(1.0/255.0, 0.0);\n\
	return ret;\n\
}	\n\
\n\
 void main()\n\
{\n\
	vec2 value = one2two(0.5);\n\
	gl_FragColor = vec4(value,value);\n\
}\n\
";