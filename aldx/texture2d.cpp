
#include "texture2d.h"

void texture2d::bind(ComPtr<ID3D11DeviceContext> context, shader_stage stage, int slot)
{
	switch (stage)
	{
	case shader_stage::Vertex:
		context->VSSetShaderResources(slot, 1, srv.GetAddressOf());
		break;
	case shader_stage::Pixel:
		context->PSSetShaderResources(slot, 1, srv.GetAddressOf());
		break;
	case shader_stage::Geometry:
		context->GSSetShaderResources(slot, 1, srv.GetAddressOf());
		break;
	case shader_stage::Domain:
		context->DSSetShaderResources(slot, 1, srv.GetAddressOf());
		break;
	case shader_stage::Hull:
		context->HSSetShaderResources(slot, 1, srv.GetAddressOf());
		break;
	case shader_stage::Compute:
		context->CSSetShaderResources(slot, 1, srv.GetAddressOf());
		break;
	}
}

void texture2d::unbind(ComPtr<ID3D11DeviceContext> context, shader_stage stage, int slot)
{
	ID3D11ShaderResourceView* nullsrvs[] = { nullptr };
	switch (stage)
	{
	case shader_stage::Vertex:
		context->VSSetShaderResources(slot, 1, nullsrvs);
		break;
	case shader_stage::Pixel:
		context->PSSetShaderResources(slot, 1, nullsrvs);
		break;
	case shader_stage::Geometry:
		context->GSSetShaderResources(slot, 1, nullsrvs);
		break;
	case shader_stage::Domain:
		context->DSSetShaderResources(slot, 1, nullsrvs);
		break;
	case shader_stage::Hull:
		context->HSSetShaderResources(slot, 1, nullsrvs);
		break;
	case shader_stage::Compute:
		context->CSSetShaderResources(slot, 1, nullsrvs);
		break;
	}
}