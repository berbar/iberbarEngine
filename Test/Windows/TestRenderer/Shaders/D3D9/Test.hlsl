
sampler2D g_targetColor;
//float4 g_targetColor;
float4 PixelKernelH[4];
float g_BlurWeight[4];
int S = 4;



float4 GG( float2 screenQuard: TEXCOORD0 ): COLOR0
{
    float3 Color = 0;
    for (int i = 0; i < S; i ++)
    {
        Color += tex2D(g_targetColor, screenQuard + PixelKernelH[i].xy).rgb * g_BlurWeight[i];
    }
    #ifdef ENABLE_ALPHA_PRESERVE
    return float4(Color, tex2D( g_targetColor, screenQuard).a );
    #else
    return float4( Color, 1.0f );
    #endif

}


technique GGTq
{
    pass p0
    {
        SRGBWRITEENABLE = true;
        PixelShader = compile ps_2_0 GG();

    }
}
