
precision highp float;

#ifdef GL_ES
varying lowp vec4 g_color;
#else
varying vec4 g_color;
#endif

void main()
{
	gl_FragColor = g_color;
}