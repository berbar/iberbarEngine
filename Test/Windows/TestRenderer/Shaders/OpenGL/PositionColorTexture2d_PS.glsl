
precision highp float;

uniform sampler2D g_textureSampler;
uniform bool g_useTexture;

#ifdef GL_ES
varying lowp vec4 g_color;
varying highp vec2 g_texcoord;
#else
varying vec4 g_color;
varying vec2 g_texcoord;
#endif

void main()
{
    if ( g_useTexture == true )
    {
        vec4 texcolor = texture2D( g_textureSampler, g_texcoord );
        gl_FragColor = texcolor * g_color;
    }
    else
    {
        gl_FragColor = g_color;
    }
}