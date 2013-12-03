
#include "States.h"


BlendState::BlendState(ComPtr<ID3D11Device> device, CD3D11_BLEND_DESC desc)
{
	device->CreateBlendState(&desc, _state.GetAddressOf());
}

BlendState::BlendState(ComPtr<ID3D11Device> device, bool blendEnable, 
		D3D11_BLEND srcBlend, D3D11_BLEND destBlend,
		D3D11_BLEND_OP blendOp, bool alphaToCoverage, 
		bool independentBlend, UINT8 rtwriteMask)
{
	CD3D11_BLEND_DESC bd(D3D11_DEFAULT);
	bd.AlphaToCoverageEnable = alphaToCoverage;
	bd.IndependentBlendEnable = independentBlend;
	bd.RenderTarget[0].BlendEnable = blendEnable;
	bd.RenderTarget[0].BlendOp = blendOp;
	bd.RenderTarget[0].SrcBlend = srcBlend;
	bd.RenderTarget[0].DestBlend = destBlend;
	bd.RenderTarget[0].RenderTargetWriteMask = rtwriteMask;
	device->CreateBlendState(&bd, _state.GetAddressOf());
}

void BlendState::OMBind(ComPtr<ID3D11DeviceContext> context, float4 blendFactor, uint32 sampleMask)
{
	float d[] = { blendFactor.x, blendFactor.y, blendFactor.z, blendFactor.w };
	context->OMSetBlendState(_state.Get(), d, sampleMask);
}

void BlendState::OMUnbind(ComPtr<ID3D11DeviceContext> context)
{
	float d[] = { 1, 1, 1, 1 };
	context->OMSetBlendState(nullptr, d, D3D11_DEFAULT_SAMPLE_MASK);
}

RasterizerState::RasterizerState(ComPtr<ID3D11Device> device, CD3D11_RASTERIZER_DESC desc)
{
	device->CreateRasterizerState(&desc, _state.GetAddressOf());
}

RasterizerState::RasterizerState(ComPtr<ID3D11Device> device, D3D11_FILL_MODE fillMode, 
		D3D11_CULL_MODE cullMode, 
		int depthBias, float slopeDepthBias, float depthBiasClamp, bool frontCounterClockwise, 
		bool depthClipEnable, bool scissorEnable)
{
	CD3D11_RASTERIZER_DESC rd(D3D11_DEFAULT);
	rd.FillMode = fillMode;
	rd.CullMode = cullMode;
	rd.DepthBias = depthBias;
	rd.SlopeScaledDepthBias = slopeDepthBias;
	rd.DepthBiasClamp = depthBiasClamp;
	rd.FrontCounterClockwise = frontCounterClockwise;
	rd.DepthClipEnable = depthClipEnable;
	rd.ScissorEnable = scissorEnable;
	device->CreateRasterizerState(&rd, _state.GetAddressOf());
}

void RasterizerState::Bind(ComPtr<ID3D11DeviceContext> context)
{
	context->RSSetState(_state.Get());
}

void RasterizerState::Unbind(ComPtr<ID3D11DeviceContext> context)
{
	context->RSSetState(nullptr);
}