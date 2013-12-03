
#include "texture2d.h"

void texture2d::bind(ComPtr<ID3D11DeviceContext> context, ShaderStage stage, int slot)
{
	switch (stage)
	{
	case ShaderStage::Vertex:
		context->VSSetShaderResources(slot, 1, srv.GetAddressOf());
		break;
	case ShaderStage::Pixel:
		context->PSSetShaderResources(slot, 1, srv.GetAddressOf());
		break;
	case ShaderStage::Geometry:
		context->GSSetShaderResources(slot, 1, srv.GetAddressOf());
		break;
	case ShaderStage::Domain:
		context->DSSetShaderResources(slot, 1, srv.GetAddressOf());
		break;
	case ShaderStage::Hull:
		context->HSSetShaderResources(slot, 1, srv.GetAddressOf());
		break;
	case ShaderStage::Compute:
		context->CSSetShaderResources(slot, 1, srv.GetAddressOf());
		break;
	}
}

void texture2d::unbind(ComPtr<ID3D11DeviceContext> context, ShaderStage stage, int slot)
{
	ID3D11ShaderResourceView* nullsrvs[] = { nullptr };
	switch (stage)
	{
	case ShaderStage::Vertex:
		context->VSSetShaderResources(slot, 1, nullsrvs);
		break;
	case ShaderStage::Pixel:
		context->PSSetShaderResources(slot, 1, nullsrvs);
		break;
	case ShaderStage::Geometry:
		context->GSSetShaderResources(slot, 1, nullsrvs);
		break;
	case ShaderStage::Domain:
		context->DSSetShaderResources(slot, 1, nullsrvs);
		break;
	case ShaderStage::Hull:
		context->HSSetShaderResources(slot, 1, nullsrvs);
		break;
	case ShaderStage::Compute:
		context->CSSetShaderResources(slot, 1, nullsrvs);
		break;
	}
}