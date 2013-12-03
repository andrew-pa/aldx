#pragma once
#include "Shader.h"
#include "TextureCube.h"

//SkyShader
// Shader for rendering Skyboxes
class SkyShader : public Shader
{
	struct wvp
	{
		float4x4 world, view, proj;
	};
public:
	SkyShader(void) { }
	SkyShader(ComPtr<ID3D11Device> device, datablob<byte>* vs_data, datablob<byte>* ps_data);
	
	inline void cameraPos(float3& p)
	{
		float4x4 trns; XMStoreFloat4x4(&trns, XMMatrixTranslation(p.x, p.y, p.z));
		wvpbufdata.world = transpose(trns);
	}
	inline void view(float4x4& m)
	{
		wvpbufdata.view = transpose(m);
	}
	inline void proj(float4x4& m)
	{
		wvpbufdata.proj = transpose(m);
	}

	inline void texture(TextureCube* t)
	{
		tex = t;
	}

	virtual void Bind(ComPtr<ID3D11DeviceContext> context);
	virtual void Unbind(ComPtr<ID3D11DeviceContext> context);
	virtual void Update(ComPtr<ID3D11DeviceContext> context);
protected:
	ComPtr<ID3D11Buffer> constbuffer_wvp;
	wvp wvpbufdata;

	TextureCube* tex;

	ComPtr<ID3D11SamplerState> sampler;

	ComPtr<ID3D11RasterizerState> rs_nocull;
	ComPtr<ID3D11DepthStencilState> dss_le;
};