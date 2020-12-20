attribute vec4 a_position;
attribute vec4 a_color;
#ifdef GL_ES
varying lowp vec4 g_color;
#else
varying vec4 g_color;
#endif
void main() {
    gl_Position = a_position;
    g_color = a_color;
}