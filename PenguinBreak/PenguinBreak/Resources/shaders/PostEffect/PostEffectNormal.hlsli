
cbuffer cbuff0 : register(b0)
{
	float time;
};
struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TECOORD;
	float4 color :COLOR;
};