"								\n\
attribute vec4 position;		\n\
attribute vec2 texcoord;		\n\
varying vec2 v_texCoord;		\n\
void main()						\n\
{								\n\
   gl_Position = position;		\n\
   v_texCoord = texcoord;		\n\
}								\n\
";