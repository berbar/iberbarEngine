
uniform bool g_rhw;
uniform mat4x4 g_matViewProjection;

attribute vec4 a_position;
attribute vec4 a_color;
attribute vec2 a_texcoord;
#ifdef GL_ES
varying lowp vec4 g_color;
varying highp vec2 g_texcoord;
#else
varying vec4 g_color;
varying vec2 g_texcoord;
#endif
void main() {
    if ( g_rhw == true )
    {
        gl_Position = a_position;
    }
    else
    {
        gl_Position = g_matViewProjection * a_position;
    }
    g_color = a_color;
    g_texcoord = a_texcoord;
}