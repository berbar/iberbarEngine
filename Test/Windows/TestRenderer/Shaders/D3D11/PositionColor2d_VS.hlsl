matrix ViewProjMatrix;
struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
};
struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 diffuse : COLOR;
};
VS_OUTPUT Main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    input.position.w = 1.0f;
    output.position = input.position;
    output.diffuse = input.color;
    return output;
}