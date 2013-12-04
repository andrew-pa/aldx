struct psinput
{
	float4 pos : SV_POSITION;
	float3 posW : POSITION;
	float2 texc : TEXCOORD0;
	float3 normW : NORMAL;
};

float4 main(psinput i) : SV_TARGET
{
	float3 c = (float3(0, 1, 0) * max(0, dot(float3(0, 1, 0), i.normW))) + float3(0, .2f, 0);
	return float4(c,0);
}