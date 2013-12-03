#pragma once
#include "helper.h"

//blend_state
// Blend State [ID3D11BlendState wrapper]
class blend_state
{
	ComPtr<ID3D11BlendState> _state;
public:
	blend_state() { }
	blend_state(ComPtr<ID3D11Device> device, CD3D11_BLEND_DESC desc);
	blend_state(ComPtr<ID3D11Device> device, bool blendEnable = true,
		D3D11_BLEND srcBlend = D3D11_BLEND_SRC_ALPHA, D3D11_BLEND destBlend = D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP blendOp = D3D11_BLEND_OP_ADD, bool alphaToCoverage = false, 
		bool independentBlend = false, UINT8 rtwriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	inline ComPtr<ID3D11BlendState>& blendState() { return _state; }

	void om_bind(ComPtr<ID3D11DeviceContext> context, float4 blendFactor = float4(1,1,1,1), uint32 sampleMask = D3D11_DEFAULT_SAMPLE_MASK);
	void om_unbind(ComPtr<ID3D11DeviceContext> context);
};

//rasterizer_state
// Rasterizer state [ID3D11RasterizerState wrapper]
class rasterizer_state
{
	ComPtr<ID3D11RasterizerState> _state;
public:
	rasterizer_state() { }
	rasterizer_state(ComPtr<ID3D11Device> device, CD3D11_RASTERIZER_DESC desc);
	rasterizer_state(ComPtr<ID3D11Device> device, D3D11_FILL_MODE fillMode = D3D11_FILL_SOLID,
		D3D11_CULL_MODE cullMode = D3D11_CULL_BACK, 
		int depthBias = 0, float slopeDepthBias = 0.0f, float depthBiasClamp = 0.0f, bool frontCounterClockwise = false, 
		bool depthClipEnable = true, bool scissorEnable = false);
	inline ComPtr<ID3D11RasterizerState>& rasterizerState() { return _state; }

	void bind(ComPtr<ID3D11DeviceContext> context);
	void unbind(ComPtr<ID3D11DeviceContext> context);
};

//sampler_state
// Texture sampler, wraps ID3D11SamplerState
class sampler_state
{
	ComPtr<ID3D11SamplerState> sampl;
	int _slot;
public:
	inline ComPtr<ID3D11SamplerState>& sampler() { return sampl; }
	inline int& slot() { return _slot; }

	sampler_state() { }

	sampler_state(ComPtr<ID3D11Device> device, int slotidx, D3D11_FILTER filter = D3D11_FILTER_ANISOTROPIC,
		D3D11_TEXTURE_ADDRESS_MODE addressMode = D3D11_TEXTURE_ADDRESS_WRAP,
		uint maxAniso = 16, float4 borderColor = float4(.5f, .5f, .5f, .5f),
		D3D11_COMPARISON_FUNC compareFunc = D3D11_COMPARISON_NEVER);

	void bind(ComPtr<ID3D11DeviceContext> context, ShaderStage stage);
	void unbind(ComPtr<ID3D11DeviceContext> context, ShaderStage stage);
};