
bool g_rhw;
matrix g_matViewProjection;
float2 g_viewport;


struct VS_INPUT
{
    float4 position : POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD0;
};
struct VS_OUTPUT
{
    float4 position : POSITION;
    float4 diffuse : COLOR;
    float2 texcoord : TEXCOORD0;
};



VS_OUTPUT Main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    if ( g_rhw == false )
    {
        output.position = mul( input.position, g_matViewProjection );
        output.position.x -= 1.0f / g_viewport.x;
        output.position.y += 1.0f / g_viewport.y;
        //output.position = mul( g_matViewProjection, input.position );
    }
    else
    {
        input.position.w = 1.0f;
        output.position = input.position;
    }
    output.diffuse = input.color;
    output.texcoord = input.texcoord;
    return output;
}