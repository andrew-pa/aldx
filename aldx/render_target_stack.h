#pragma once
#include "helper.h"

struct render_target
{
	ComPtr<ID3D11RenderTargetView> render_targetv;
	ComPtr<ID3D11DepthStencilView> depth_stencil;
	D3D11_VIEWPORT viewport;
	render_target(ComPtr<ID3D11RenderTargetView> rtv, ComPtr<ID3D11DepthStencilView> dsv, D3D11_VIEWPORT vp)
		: render_targetv(rtv), depth_stencil(dsv), viewport(vp)
	{
	}
};

class render_target_stack
{
public:
	propr(virtual render_target, current_render_target, = 0;)
	inline virtual void pop_render_target() = 0;
	inline virtual void push_render_target(ComPtr<ID3D11RenderTargetView> rtv, ComPtr<ID3D11DepthStencilView> dsv,
		D3D11_VIEWPORT vp) = 0;
	inline virtual void set_render_target() = 0;
};