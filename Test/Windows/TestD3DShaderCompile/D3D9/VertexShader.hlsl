

struct TestStruct
{
	float a;
    float4 b;
	float4x4 c;
};

int g_Int;
TestStruct g_TestStruct;



float4 main( float4 pos : POSITION ) : SV_POSITION
{
    pos.a = g_Int;
    pos.y = g_TestStruct.a;
	return pos;
}