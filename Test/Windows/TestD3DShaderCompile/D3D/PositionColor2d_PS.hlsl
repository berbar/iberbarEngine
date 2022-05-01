
struct PixelInputType
{
	float4 pos : POSITION;
	float4 color : COLOR;
};
float4 Main(PixelInputType input) : COLOR
{
	return input.color;
}