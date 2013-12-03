
#include "SkyShader.h"


static const D3D11_INPUT_ELEMENT_DESC Layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

SkyShader::SkyShader(ComPtr<ID3D11Device> device, 
						 datablob<byte>* vs_data, datablob<byte>* ps_data)
						 : Shader(device, vs_data, ps_data, Layout, 3)
{
	CD3D11_BUFFER_DESC cbd(sizeof(wvp), D3D11_BIND_CONSTANT_BUFFER);
	chr(device->CreateBuffer(&cbd, nullptr, &constbuffer_wvp));
	ZeroMemory(&wvpbufdata, sizeof(wvp));

	D3D11_SAMPLER_DESC sampd;
	ZeroMemory(&sampd, sizeof(D3D11_SAMPLER_DESC));
	sampd.Filter =  D3D11_FILTER_ANISOTROPIC;
	sampd.AddressU = sampd.AddressV = sampd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampd.MipLODBias = 0.0f;
	sampd.MaxAnisotropy = 16;
	sampd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampd.BorderColor[0] = sampd.BorderColor[1] = sampd.BorderColor[2] = sampd.BorderColor[3] = 0.5f;
	sampd.MinLOD = 0;
	sampd.MaxLOD = D3D11_FLOAT32_MAX;
	chr(device->CreateSamplerState(&sampd, &sampler));

	D3D11_RASTERIZER_DESC rsd;
	ZeroMemory(&rsd, sizeof(rsd));
	rsd.CullMode = D3D11_CULL_NONE;
	rsd.FillMode = D3D11_FILL_SOLID;
	D3D11_DEPTH_STENCIL_DESC dsd;
	ZeroMemory(&dsd, sizeof(dsd));
	dsd.DepthEnable = true;
	dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateRasterizerState(&rsd, rs_nocull.GetAddressOf());
	device->CreateDepthStencilState(&dsd, dss_le.GetAddressOf());
}

void SkyShader::Bind(ComPtr<ID3D11DeviceContext> context)
{
	Shader::Bind(context);
	context->RSSetState(rs_nocull.Get());
	context->OMSetDepthStencilState(dss_le.Get(), 0);
	context->VSSetconstant_buffers(0, 1, constbuffer_wvp.GetAddressOf());
	context->PSSetSamplers(0, 1, sampler.GetAddressOf());
	tex->Bind(context, ShaderStage::Pixel);
	Update(context);
}

void SkyShader::Unbind(ComPtr<ID3D11DeviceContext> context)
{
	Shader::Unbind(context);
	context->RSSetState(nullptr);
	context->OMSetDepthStencilState(nullptr, 0);
	static ID3D11Buffer* nullbufs[] = { nullptr };
	static ID3D11SamplerState* nullsmps[] = { nullptr };
	context->VSSetconstant_buffers(0, 1, nullbufs);
	context->PSSetSamplers(0, 1, nullsmps);
	tex->Unbind(context, ShaderStage::Pixel);
}

void SkyShader::Update(ComPtr<ID3D11DeviceContext> context)
{
	context->UpdateSubresource(constbuffer_wvp.Get(), 0, NULL, &wvpbufdata, 0, 0);
}