
#include "shadow_map.h"

namespace aldx
{
	shadow_map::shadow_map(ComPtr<ID3D11Device> device, float2 size, int depthBias)
		: depth_render_texture(device, size)
	{
		CD3D11_RASTERIZER_DESC rsd(D3D11_FILL_SOLID, D3D11_CULL_BACK, FALSE,
			depthBias, 0.0f, 1.0f, TRUE, FALSE, FALSE, FALSE);
		chr(device->CreateRasterizerState(&rsd, shadowmap_rsstate.GetAddressOf()));
	}

	void shadow_map::update_proj(float4 lightDirection, float radius, float lightfov)
	{
		static const XMMATRIX T(
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f);
		auto p = XMMatrixPerspectiveFovLH(lightfov, _size.x / _size.y, 0.1f, 1000.f);
		XMVECTOR eye = XMLoadFloat4(&lightDirection);
		eye = eye * float3(radius);
		auto v = XMMatrixLookAtLH(eye, XMVectorSet(0, 0, 0, 1), XMVectorSet(0, 1, 0, 1));
		XMStoreFloat4x4(&_shadowProj, p);
		XMStoreFloat4x4(&_shadowView, v);
		XMStoreFloat4x4(&_shadowViewProjTex, v*p*T);
	}

	void shadow_map::om_bind(ComPtr<ID3D11DeviceContext> context)
	{
		context->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
		depth_render_texture::om_bind(context);
		context->RSSetState(shadowmap_rsstate.Get());
	}

	void shadow_map::om_unbind(ComPtr<ID3D11DeviceContext> context)
	{
		depth_render_texture::om_unbind(context);
		context->RSSetState(nullptr);
	}
}
//namespace Oldshadow_map
//{
//shadow_map::shadow_map(ComPtr<ID3D11Device> device, float mapH, float mapW, float depthB, float depthBClamp, float DBSlopeScale,
//					 float sceneRadius) 
//					 : h(mapH), w(mapW), db(depthB), dbc(depthBClamp), ssdb(DBSlopeScale),
//					 srad(sceneRadius)
//{
//	
//	CD3D11_TEXTURE2D_DESC smtd(DXGI_FORMAT_R24G8_TYPELESS, (UINT)mapH, (UINT)mapW, 1U, 1U);
//	smtd.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
//	CD3D11_SHADER_RESOURCE_VIEW_DESC srvd;
//	srvd.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
//	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//	srvd.texture2d.MipLevels = smtd.MipLevels;
//	srvd.texture2d.MostDetailedMip = 0;
//	texture = new texture2d(device, smtd, srvd);
//	
//	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
//	dsvd.Flags = 0;
//	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
//	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
//	dsvd.texture2d.MipSlice = 0;
//	chr(device->CreateDepthStencilView(texture->texture2D().Get(), &dsvd, _shadow_mapDSV.GetAddressOf()));
//
//	CD3D11_RASTERIZER_DESC rsDed;
//	rsDed.FillMode = D3D11_FILL_SOLID;
//	rsDed.CullMode = D3D11_CULL_BACK;
//	rsDed.FrontCounterClockwise = false;
//	rsDed.DepthBias = (INT)depthB;
//	rsDed.DepthBiasClamp = depthBClamp;
//	rsDed.SlopeScaledDepthBias = DBSlopeScale;
//	rsDed.DepthClipEnable = true;
//	rsDed.ScissorEnable = false;
//	rsDed.MultisampleEnable = false;
//	rsDed.AntialiasedLineEnable = false;
//	chr(device->CreateRasterizerState(&rsDed, _rsDepth.GetAddressOf()));
//
//	buildShader = new Buildshadow_mapShader(device, 
//		DX::ReadDataFromPackage(L"shadow_map_vs.cso"));
//}
//
//void shadow_map::Update(float3 lightDirection, float3 lightTarg)
//{
//	float3 lightPos = -2.0f*srad*lightDirection;
//	load3(lightTarg);
//	load3as(lightDirection, LD);
//	load3as(lightPos, LP);
//	matrix view = XMMatrixLookAtLH(LP, vlightTarg+XMVectorSet(0, .1f, 0, 0), XMVectorSet(0, 1, 0, 0));
//	
//	float3 ltls;
//	store3(ltls, XMVector3TransformCoord(vlightTarg, view));
//
//	float l = ltls.x - srad;
//	float b = ltls.y - srad;
//	float n = ltls.z - srad;
//	float r = ltls.x + srad;
//	float t = ltls.y + srad;
//	float f = ltls.z + srad;
//	matrix proj = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);
//
//	XMMATRIX T(
//		0.5f, 0.0f, 0.0f, 0.0f,
//		0.0f, -0.5f, 0.0f, 0.0f,
//		0.0f, 0.0f, 1.0f, 0.0f,
//		0.5f, 0.5f, 0.0f, 1.0f);
//	matrix lvpt = view * proj * T;
//
//	store4x4(lightview, view);
//	store4x4(lightproj, proj);
//	store4x4(lightprojtex, lvpt);
//}
//
//void shadow_map::Bind(ComPtr<ID3D11DeviceContext> context)
//{
//	CD3D11_VIEWPORT light_vp(0.f, 0.f, w, h);
//	context->RSSetViewports(1, &light_vp);
//	ID3D11RenderTargetView* rts[1] = {nullptr};
//	context->ClearDepthStencilView(_shadow_mapDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
//	context->OMSetRenderTargets(1, rts, _shadow_mapDSV.Get());
//	context->RSSetState(_rsDepth.Get());
//	buildShader->view(lightview);
//	buildShader->proj(lightproj);
//}
//}