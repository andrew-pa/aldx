#pragma once
#include "helper.h"
#include "shader.h"
#include "texture2d.h"

////decap !!
//struct directional_light
//{
//	float4 direction;
//	float4 diffuse;
//	float4 specular;
//
//	directional_light() { }
//	directional_light(float4 d, float4 dc, float4 sc)
//		: direction(d), diffuse(dc), specular(sc)
//	{
//	}
//};
//
////decap !!
//struct point_light
//{
//	float4 position;
//	float4 diffuse;
//	float4 specular;
//
//	point_light() { }
//	point_light(float4 d, float4 dc, float4 sc)
//		: position(d), diffuse(dc), specular(sc)
//	{
//	}
//};

//material
// Simple Direct Lighting material
struct material
{
	float4 diffuse, specular, ambient;
	material()
		: diffuse(.7f, .7f, .7f, 1), specular(1, 1, 1, 0), ambient(.2f, .2f, .2f, 1)
	{ }
	material(float4 d, float4 s, float4 a)
		: diffuse(d), specular(s), ambient(a)
	{
	}
};

//IRenderShader
// Interface for all shaders involved with rendering. Effectively dictated bygame_object
class render_shader : public shader
{
public:
	render_shader() { }
	render_shader(ComPtr<ID3D11Device> device, datablob<byte>* vs_data, datablob<byte>* ps_data,
		const D3D11_INPUT_ELEMENT_DESC inputLayout[], int inputLayoutLength)
		: shader(device, vs_data, ps_data, inputLayout, inputLayoutLength) { }
	virtual void world(const float4x4& m) = 0;
	virtual void view(const float4x4& m) = 0;
	virtual void proj(const float4x4& m) = 0;
	virtual void setTexture(Texture2D* tex) = 0;
	virtual void setMaterial(const material& mat) = 0;
};


////no longer supported - see negdx's phongshader
////!!DECAPRECATED!! DO NOT USE - DOESN'T HAVE UNBIND, NEVER UPDATED
//class BasicShader : public IRenderShader
//{
//	struct wvp
//	{
//		float4x4 world, worldintp, view, proj, shadowProj, shadowProj1, shadowProj2;
//	};
//
//	struct pxd1
//	{
//		float3 eyeposW;
//		int numdirlights;
//		int numpntlights;
//		float4 opts;
//		directional_light dir_lights[3];
//		point_light pnt_lights[5];
//	};
//public:
//	BasicShader(void) { }
//	BasicShader(ComPtr<ID3D11Device> device, datablob<byte>* vs_data, datablob<byte>* ps_data);
//	
//	virtual inline void world(const float4x4& m) override
//	{
//		wvpdata_change = true;
//		wvpbufdata.world = transpose(m);
//		load4x4(m);
//		store4x4(wvpbufdata.worldintp, (XMMatrixInverse(nullptr, (mm))));
//	}
//	virtual inline void view(const float4x4& m) override
//	{
//		wvpbufdata.view = transpose(m);
//		wvpdata_change = true;
//	}
//	virtual inline void proj(const float4x4& m) override
//	{
//		wvpbufdata.proj = transpose(m);
//		wvpdata_change = true;
//	}
//
//	inline void cameraPos(float3 p)
//	{
//		pxd1bufdata.eyeposW = p;
//		pxd1data_change = true;
//	}
//
//	inline void setDirectionalLight(directional_light l, int idx)
//	{
//		pxd1bufdata.dir_lights[idx] = l;
//		pxd1data_change = true;
//	}
//	inline void setNumDirectionalLights(int n)
//	{
//		pxd1bufdata.numdirlights = n;
//		pxd1data_change = true;
//	}
//
//	inline void num_pointlights(int n)
//	{
//		pxd1bufdata.numpntlights = n;
//		pxd1data_change = true;
//	}
//
//	inline void setPointLight(point_light l, int idx)
//	{
//		pxd1bufdata.pnt_lights[idx] = l;
//		pxd1data_change = true;
//	}
//
//	virtual inline void setMaterial(const material& m) override
//	{
//		mat = m;
//		mat_change = true;
//	}
//	virtual inline void setDefaultMaterial()
//	{
//		setMaterial(material(float4(.7f,.7f,.7f,1), float4(1,1,1,0), float4(.2f, .2f, .2f, 1)));
//	}
//
//	virtual inline void setTexture(Texture2D* tex) override
//	{
//		difTex = tex->shaderResourceView();
//		dtex_change = true;
//	}
//
//	inline void useShadows(bool v)
//	{
//		pxd1bufdata.opts.x = (v ? 1.0f : 0.0f);
//		pxd1data_change = true;
//	}
//
//	inline void setShadowMap(ComPtr<ID3D11ShaderResourceView> tex, int idx)
//	{
//		shadowMap[idx] = tex;	
//		shmap_change = true;
//	}
//
//	inline void shadowproj(const float4x4& m, int idx)
//	{
//		wvpdata_change = true;
//		if(idx == 0)
//			wvpbufdata.shadowProj = transpose(m);
//		else if(idx == 1)
//			wvpbufdata.shadowProj1 = transpose(m);
//		else if(idx == 2)
//			wvpbufdata.shadowProj2 = transpose(m);
//	}
//
//	virtual void Bind(ComPtr<ID3D11DeviceContext> context);
//	virtual void Update(ComPtr<ID3D11DeviceContext> context);
//protected:
//	ComPtr<ID3D11Buffer> constbuffer_wvp;
//	wvp wvpbufdata;
//	bool wvpdata_change;
//
//	ComPtr<ID3D11Buffer> constbuffer_pxd1;
//	pxd1 pxd1bufdata;
//	bool pxd1data_change;
//
//	ComPtr<ID3D11Buffer> constbuffer_mat;
//	material mat;
//	bool mat_change;
//
//	ComPtr<ID3D11SamplerState> sampler;
//	ComPtr<ID3D11SamplerState> samplercomp;
//	ComPtr<ID3D11ShaderResourceView> difTex;
//	bool dtex_change;
//	ComPtr<ID3D11ShaderResourceView> shadowMap[3];
//	bool shmap_change;
//};
//
