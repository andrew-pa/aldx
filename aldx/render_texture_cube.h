#pragma once
#include "helper.h"
#include "texture_cube.h"

//render_texture_cube
// TextureCube where you can render to each of the faces
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

	inline const ComPtr<ID3D11RenderTargetView>& render_target_view(int idx) const 
	{
		return rtvs[idx];
	}

	inline const ComPtr<ID3D11DepthStencilView>& depth_stencil_view() const
	{ return dsv; }
};

