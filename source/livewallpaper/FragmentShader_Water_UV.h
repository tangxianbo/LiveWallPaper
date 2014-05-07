"												\n\
precision lowp float;							\n\
varying vec2 vCoord;							\n\
uniform sampler2D water;						\n\
												\n\
void main()										\n\
{                                               \n\
	gl_FragColor = texture2D(water,vCoord);		\n\
}												\n\
";