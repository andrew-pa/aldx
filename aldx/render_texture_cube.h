#pragma once
#include "helper.h"
#include "texture_cube.h"
#include "render_target_stack.h"

namespace aldx
{
	//render_texture_cube
	// texture_cube where you can render to each of the faces
	class render_texture_cube :
		public texture_cube
	{
		ComPtr<ID3D11RenderTargetView> rtvs[6];
		ComPtr<ID3D11DepthStencilView> dsv;
		float _cmsize;
	public:
		render_texture_cube() { }
		render_texture_cube(ComPtr<ID3D11Device> device, uint cubeMapSize);
		void om_bind(ComPtr<ID3D11DeviceContext> context, int idx);
		void om_unbind(ComPtr<ID3D11DeviceContext> context, int idx);

		void push(render_target_stack* rts, int idx);

		inline const ComPtr<ID3D11RenderTargetView>& render_target_view(int idx) const
		{
			return rtvs[idx];
		}

		inline const ComPtr<ID3D11DepthStencilView>& depth_stencil_view() const
		{
			return dsv;
		}
	};

	//depth_render_texture_cube
	//possibly the class with the longest name. useful for point light shadow mapping. 
	class depth_render_texture_cube : public texture_cube
	{
		ComPtr<ID3D11DepthStencilView> dsvs[6];
		float _cmsize;
	public:
		depth_render_texture_cube() {}
		depth_render_texture_cube(ComPtr<ID3D11Device> device, uint cubemapsize);
		void om_bind(ComPtr<ID3D11DeviceContext> context, int idx);
		void om_unbind(ComPtr<ID3D11DeviceContext> context, int idx);

		void push(render_target_stack* rts, int idx);

		inline const ComPtr<ID3D11DepthStencilView>& depth_stencil_view(int idx) const
		{
			return dsvs[idx];
		}
	};
}
