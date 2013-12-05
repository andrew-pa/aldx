
#include "render_texture_cube.h"


render_texture_cube::render_texture_cube(ComPtr<ID3D11Device> device, uint cubeMapSize)
	: texture_cube(device, CD3D11_TEXTURE2D_DESC(
	DXGI_FORMAT_R8G8B8A8_UNORM, cubeMapSize,
	cubeMapSize, 6, 1, 
	D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
	D3D11_USAGE_DEFAULT)), _cmsize((float)cubeMapSize)
{
	CD3D11_RENDER_TARGET_VIEW_DESC rtvd(D3D11_RTV_DIMENSION_TEXTURE2DARRAY,
		DXGI_FORMAT_R8G8B8A8_UNORM);
	rtvd.Texture2DArray.ArraySize = 1;
	rtvd.Texture2DArray.MipSlice = 0;
	for(int i = 0; i < 6; ++i)
	{
		rtvd.Texture2DArray.FirstArraySlice = i;
		chr(device->CreateRenderTargetView(texd.Get(), &rtvd, &rtvs[i]));
	}

	CD3D11_TEXTURE2D_DESC dstd(DXGI_FORMAT_D32_FLOAT, 
		cubeMapSize, cubeMapSize, 1, 1, D3D11_BIND_DEPTH_STENCIL);
	ComPtr<ID3D11Texture2D> te2d;
	chr(device->CreateTexture2D(&dstd, nullptr, te2d.GetAddressOf()));
	CD3D11_DEPTH_STENCIL_VIEW_DESC dsvd(
		D3D11_DSV_DIMENSION_TEXTURE2D, dstd.Format);
	chr(device->CreateDepthStencilView(te2d.Get(), &dsvd, dsv.GetAddressOf()));
}

void render_texture_cube::om_bind(ComPtr<ID3D11DeviceContext> context, int idx)
{
	CD3D11_VIEWPORT vp(0.f, 0.f, _cmsize, _cmsize);
	context->RSSetViewports(1, &vp);
	context->OMSetRenderTargets(1, rtvs[idx].GetAddressOf(), dsv.Get());
}

void render_texture_cube::push(render_target_stack* rts, int idx)
{
	rts->push_render_target(rtvs[idx], dsv, CD3D11_VIEWPORT(0.f, 0.f, _cmsize, _cmsize));
}

void render_texture_cube::om_unbind(ComPtr<ID3D11DeviceContext> context, int idx)
{
	context->RSSetViewports(1, nullptr);
	ID3D11RenderTargetView* nullrtv[] = { nullptr };
	context->OMSetRenderTargets(1, nullrtv, nullptr);
}