
int g_rhw;
matrix g_matViewProjection;
float g_viewport_w;
float g_viewport_h;


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
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.position = float4(input.position, 1.0f);
    // if ( g_rhw == 0 )
    // {
    //     output.position = mul( output.position, g_matViewProjection );
    //     output.position.x -= 1.0f / g_viewport_w;
    //     output.position.y += 1.0f / g_viewport_h;
    //     //output.position = mul( g_matViewProjection, input.position );
    // }
    // else
    // {
    //     output.position.w = 1.0f;
    //     //output.position = input.position;
    // }
    output.diffuse = input.color;
    output.texcoord = input.texcoord;
    return output;
}