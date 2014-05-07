"																							\n\
attribute vec2 position;																	\n\
attribute vec2 coord;																		\n\
varying vec2 vCoord;																		\n\
																							\n\
void main()																					\n\
{                                                                                           \n\
	vCoord = coord;																			\n\
	gl_Position = vec4(position, 0.0, 1.0);													\n\
}																							\n\
";