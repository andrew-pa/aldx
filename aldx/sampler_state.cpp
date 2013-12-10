
#include "states.h"


sampler_state::sampler_state(ComPtr<ID3D11Device> device, int slotidx, D3D11_FILTER filter, 
		D3D11_TEXTURE_ADDRESS_MODE addressMode, 
		uint maxAniso, float4 borderColor,
		D3D11_COMPARISON_FUNC compareFunc)
		: _slot(slotidx)
{
	D3D11_SAMPLER_DESC smd; ZeroMemory(&smd, sizeof(smd));
	smd.Filter = filter;
	smd.AddressU = smd.AddressV = smd.AddressW = addressMode;
	smd.MipLODBias = 0.0f;
	smd.MaxAnisotropy = maxAniso;
	smd.ComparisonFunc = compareFunc;
	smd.BorderColor[0] = borderColor.x; smd.BorderColor[1] = borderColor.y;
	smd.BorderColor[2] = borderColor.z; smd.BorderColor[3] = borderColor.w;
	smd.MinLOD = 0;
	smd.MaxLOD = D3D11_FLOAT32_MAX;
	chr(device->CreateSamplerState(&smd, sampl.GetAddressOf()));
}

void sampler_state::bind(ComPtr<ID3D11DeviceContext> context, shader_stage stage)
{
	switch (stage)
	{
	case shader_stage::Vertex:
		context->VSSetSamplers(_slot, 1, sampl.GetAddressOf());
		break;
	case shader_stage::Pixel:
		context->PSSetSamplers(_slot, 1, sampl.GetAddressOf());
		break;
	case shader_stage::Geometry:
		context->GSSetSamplers(_slot, 1, sampl.GetAddressOf());
		break;
	case shader_stage::Domain:
		context->DSSetSamplers(_slot, 1, sampl.GetAddressOf());
		break;
	case shader_stage::Hull:
		context->HSSetSamplers(_slot, 1, sampl.GetAddressOf());
		break;
	case shader_stage::Compute:
		context->CSSetSamplers(_slot, 1, sampl.GetAddressOf());
		break;
	}
}

void sampler_state::unbind(ComPtr<ID3D11DeviceContext> context, shader_stage stage)
{
	ID3D11SamplerState* nullsmps[] = { nullptr };
	switch (stage)
	{
	case shader_stage::Vertex:
		context->VSSetSamplers(_slot, 1, nullsmps);
		break;
	case shader_stage::Pixel:
		context->PSSetSamplers(_slot, 1, nullsmps);
		break;
	case shader_stage::Geometry:
		context->GSSetSamplers(_slot, 1, nullsmps);
		break;
	case shader_stage::Domain:
		context->DSSetSamplers(_slot, 1, nullsmps);
		break;
	case shader_stage::Hull:
		context->HSSetSamplers(_slot, 1, nullsmps);
		break;
	case shader_stage::Compute:
		context->CSSetSamplers(_slot, 1, nullsmps);
		break;
	}
}
