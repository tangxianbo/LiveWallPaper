"														\n\
precision mediump float;								\n\
varying vec2 v_texCoord;								\n\
uniform sampler2D s_texture;							\n\
void main()												\n\
{														\n\
	gl_FragColor = texture2D( s_texture, v_texCoord);	\n\
}														\n\
";