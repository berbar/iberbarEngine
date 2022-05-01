matrix ViewProjMatrix;
struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};
struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 diffuse : COLOR;
    float2 texcoord : TEXCOORD;
};
VS_OUTPUT Main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    //input.position.w = 1.0f;
    output.position = float4(input.position, 1.0f);
    output.diffuse = input.color;
    return output;
}