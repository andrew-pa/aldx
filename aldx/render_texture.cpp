
#include "render_texture.h"


render_texture::render_texture(ComPtr<ID3D11Device> device, float2 size)
	: texture2d(device, 
		CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R32G32B32A32_FLOAT, (UINT)size.x, (UINT)size.y, 
		1, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE)), _size(size)
{
	CD3D11_TEXTURE2D_DESC dstd(DXGI_FORMAT_R24G8_TYPELESS, (UINT)size.x, (UINT)size.y);
	dstd.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	ComPtr<ID3D11Texture2D> dst;
	chr(device->CreateTexture2D(&dstd, nullptr, dst.GetAddressOf()));
	
	CD3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	dsvd.Flags = 0;
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice= 0;
	chr(device->CreateDepthStencilView(dst.Get(), &dsvd, dsv.GetAddressOf()));

	CD3D11_RENDER_TARGET_VIEW_DESC rsvd(D3D11_RTV_DIMENSION_TEXTURE2D, DXGI_FORMAT_R32G32B32A32_FLOAT);
	chr(device->CreateRenderTargetView(texd.Get(), &rsvd, rtv.GetAddressOf()));
}

void render_texture::om_bind(ComPtr<ID3D11DeviceContext> context)
{
	CD3D11_VIEWPORT vp(0.f, 0.f, _size.x, _size.y);
	context->RSSetViewports(1, &vp);
	context->OMSetRenderTargets(1, rtv.GetAddressOf(), dsv.Get());
}

void render_texture::push(render_target_stack* rts)
{
	rts->push_render_target(rtv, dsv, CD3D11_VIEWPORT(0.f, 0.f, _size.x, _size.y));
}

void render_texture::om_unbind(ComPtr<ID3D11DeviceContext> context)
{
	ID3D11RenderTargetView* nullrtvs[] = { nullptr };
	context->OMSetRenderTargets(1, nullrtvs, dsv.Get());
}

depth_render_texture::depth_render_texture(ComPtr<ID3D11Device> device, float2 size)
	: texture2d(device, 
		CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R24G8_TYPELESS, (UINT)size.x, (UINT)size.y, 
			1, 1, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE), 
			CD3D11_SHADER_RESOURCE_VIEW_DESC(D3D11_SRV_DIMENSION_TEXTURE2D, DXGI_FORMAT_R24_UNORM_X8_TYPELESS,
			0, 1)), _size(size)
{
	CD3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	dsvd.Flags = 0;
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;
	chr(device->CreateDepthStencilView(texd.Get(), &dsvd, dsv.GetAddressOf()));
}

void depth_render_texture::om_bind(ComPtr<ID3D11DeviceContext> context)
{
	CD3D11_VIEWPORT vp(0.f, 0.f, _size.x, _size.y);
	context->RSSetViewports(1, &vp);
	ID3D11RenderTargetView* nulltargets[] = { nullptr };
	context->OMSetRenderTargets(1, nulltargets, dsv.Get());
}

void depth_render_texture::push(render_target_stack* rts)
{
	rts->push_render_target(nullptr, dsv, CD3D11_VIEWPORT(0.f, 0.f, _size.x, _size.y));
}

void depth_render_texture::om_unbind(ComPtr<ID3D11DeviceContext> context)
{
	ID3D11RenderTargetView* nulltargets[] = { nullptr };
	context->OMSetRenderTargets(1, nulltargets, nullptr);
}
