#pragma once
#include "texture2d.h"
#include "render_target_stack.h"

namespace aldx
{
	//render_texture
	// Texture that can be bound to the OM stage of the pipeline, so you can render to it
	class render_texture :
		public texture2d
	{
	protected:
		ComPtr<ID3D11DepthStencilView> dsv;
		ComPtr<ID3D11RenderTargetView> rtv;
		float2 _size;
	public:
		render_texture() { }
		render_texture(ComPtr<ID3D11Device> device, float2 size);

		//decap
		virtual void om_bind(ComPtr<ID3D11DeviceContext> context);
		//decap
		virtual void om_unbind(ComPtr<ID3D11DeviceContext> context);

		virtual void push(render_target_stack* rts);

		propr(ComPtr<ID3D11DepthStencilView>, depth_stencil, { return dsv; })
			propr(ComPtr<ID3D11RenderTargetView>, render_targetv, { return rtv; })
	};

	//depth_render_texture
	// render_texture that only renders depth, useful for shadow mapping, etc.
	class depth_render_texture : public texture2d
	{
	protected:
		ComPtr<ID3D11DepthStencilView> dsv;
		float2 _size;
	public:
		depth_render_texture() { }
		depth_render_texture(ComPtr<ID3D11Device> device, float2 size);

		virtual void om_bind(ComPtr<ID3D11DeviceContext> context);
		virtual void om_unbind(ComPtr<ID3D11DeviceContext> context);

		virtual void push(render_target_stack* rts);

		propr(ComPtr<ID3D11DepthStencilView>, depth_stencil, { return dsv; });
	};
}