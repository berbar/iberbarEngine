matrix ViewProjMatrix;
struct VS_INPUT
{
    vector position : POSITION;
    vector color : COLOR;
};
struct VS_OUTPUT
{
    vector position : POSITION;
    vector diffuse : COLOR;
};
VS_OUTPUT Main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    input.position.w = 1.0f;
    output.position = input.position;
    output.diffuse = input.color;
    return output;
}