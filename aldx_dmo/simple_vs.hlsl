#pragma pack_matrix( row_major )

cbuffer ModelDepd : register(b0)
{
	float4x4 world;
	float4x4 worldInvTrp;
};
cbuffer CameraDepd : register(b1)
{
	float4x4 view;
	float4x4 proj;
	float4 cameraPos;
};

struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 norm : NORMAL;
	float2 texc : TEXCOORD;
};

struct VertexShaderOutput
{
	float4 pos : SV_POSITION;
	float3 posW : POSITION;
	float2 texc : TEXCOORD0;
	float3 normW : NORMAL;

};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	float4 pos = float4(input.pos, 1.0f);
	pos = mul(pos, world);
	output.posW = pos;
	pos = mul(pos, view);
	pos = mul(pos, proj);
	output.pos = pos;
	output.texc = input.texc;
	output.normW = mul(input.norm, worldInvTrp);
	return output;
}
