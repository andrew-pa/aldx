#pragma once
#include "helper.h"
#include "render_shader.h"
#include "render_texture.h"

//shadow_map
// DepthRenderTexture with special RasterizerState (using DepthBias) and a Light Projection
class shadow_map : public depth_render_texture
{
	ComPtr<ID3D11RasterizerState> shadowmap_rsstate;

	float4x4 _shadowViewProjTex;
	float4x4 _shadowView;
	float4x4 _shadowProj;
public:
	shadow_map() { }

	shadow_map(ComPtr<ID3D11Device> device, float2 size, int depthBias);

	//Update the ProjTex, Proj, and View, matrices for a particular directional light ("light" is the direction)
	void update_proj(float4 light, float radius, float lightfov);

	virtual void om_bind(ComPtr<ID3D11DeviceContext> context);
	virtual void om_unbind(ComPtr<ID3D11DeviceContext> context);
	const float4x4& shadow_proj_tex() const { return _shadowViewProjTex; }
	const float4x4& shadow_proj() const { return _shadowProj; }
	const float4x4& shadow_view() const { return _shadowView; }

};














namespace OldShadowMap 
{
	static const D3D11_INPUT_ELEMENT_DESC ___ShadowMap_Layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
class ShadowMap
{
public:
	ShadowMap() {} 
	ShadowMap(ComPtr<ID3D11Device> device, float mapH, float mapW, float depthB, float depthBClamp, float DBSlopeScale,
		float sceneRadius);

	void Update(float3 lightDirection, float3 lightTarget = float3(0,0,0));
	void Bind(ComPtr<ID3D11DeviceContext> context);
	
	inline IRenderShader* shadowMapBuildShader() { return buildShader; }
	inline float mapHeight() const { return h; } 
	inline float mapWidth() const { return w; } 
	inline float depthBias() const { return db; } 
	inline float depthBiasClamp() const { return dbc; } 
	inline float slopeScaledDepthBias() const { return ssdb; } 
	proprw(float, sceneRadius, { return srad; })
	inline Texture2D* shadowMapTexture() { return texture; } 
	inline const float4x4& lightProjTex() const { return lightprojtex; }

//protected:
	float h, w, db, dbc, ssdb, srad;
	ComPtr<ID3D11DepthStencilView> _shadowMapDSV;
	Texture2D* texture;
	ComPtr<ID3D11RasterizerState> _rsDepth;
	IRenderShader* buildShader;
	float4x4 lightview, lightproj, lightprojtex;
	class BuildShadowMapShader : public IRenderShader
	{
		struct wvp
		{
			float4x4 world, view, proj;
		};
		wvp wvpdata;
		ComPtr<ID3D11Buffer> cb_wvp;
	public:
		BuildShadowMapShader(ComPtr<ID3D11Device> device, datablob<byte>* vs_data)
			: IRenderShader(device, vs_data, nullptr, ___ShadowMap_Layout, 3)
		{
			CD3D11_BUFFER_DESC ccc(sizeof(wvp), D3D11_BIND_CONSTANT_BUFFER);
			chr(device->CreateBuffer(&ccc, nullptr, &cb_wvp));
			ZeroMemory(&wvpdata, sizeof(wvp));
		}
		virtual void Bind(ComPtr<ID3D11DeviceContext> context) override
		{
			IRenderShader::Bind(context);
			context->VSSetconstant_buffers(0, 1, cb_wvp.GetAddressOf());
			Update(context);
		}
		virtual void Update(ComPtr<ID3D11DeviceContext> context) override
		{
			if(context == nullptr) return;
			context->UpdateSubresource(cb_wvp.Get(), 0, NULL, &wvpdata, 0, 0);
		}
		virtual inline void world(const float4x4& m) override
		{
			wvpdata.world = transpose(m);
		}
		virtual inline void view(const float4x4& m) override
		{
			wvpdata.view = transpose(m);
		}
		virtual inline void proj(const float4x4& m) override
		{
			wvpdata.proj = transpose(m);
		}
		virtual inline void setTexture(Texture2D* texsrv) override
		{
		}
		virtual inline void setMaterial(const material& m) override { } 
	};
};

}