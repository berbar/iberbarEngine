

Texture2D g_texture : register(t0);
SamplerState g_textureSampler : register(s0);
bool g_useTexture;


struct PixelInputType
{
    float4 pos : POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD0;
};
float4 Main(PixelInputType input) : SV_TARGET
{
    if (g_useTexture == true)
    {
        return g_texture.Sample(g_textureSampler, input.texcoord) * input.color;
    }
		
    return input.color;
}