
struct PixelInputType
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};
float4 Main(PixelInputType input) : SV_TARGET
{
	return input.color;
}