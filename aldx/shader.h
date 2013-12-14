#pragma once
#include "helper.h"

namespace aldx
{

	//IShader
	// Base class for all shaders, even compute shaders
	class ishader
	{
	public:
		//Binds the shader to the context
		virtual void bind(ComPtr<ID3D11DeviceContext> context) = 0;

		//Unbinds everything bound to the Context, effectively calls all context functions with nullptr instead of the object
		virtual void unbind(ComPtr<ID3D11DeviceContext> context) = 0;
	};

	//shader
	// Base class for all shaders with a Vertex and Pixel shader
	class shader : public ishader
	{
	public:
		shader(void);
		shader(ComPtr<ID3D11Device> device, datablob<byte>* vs_shaderData,
			datablob<byte>* ps_shaderData, const D3D11_INPUT_ELEMENT_DESC inputLayout[], int inputLayoutLength);

		//Bind the vertex and pixel shaders, and input layout
		virtual void bind(ComPtr<ID3D11DeviceContext> context);

		//Unbind everything
		virtual void unbind(ComPtr<ID3D11DeviceContext> context);

		//Update any constant buffers, textures, etc.
		virtual void update(ComPtr<ID3D11DeviceContext> context) { }


		proprw(ComPtr<ID3D11VertexShader>, vertex_shader, { return vertexShader; })
			proprw(ComPtr<ID3D11PixelShader>, pixel_shader, { return pixelShader; })
			proprw(ComPtr<ID3D11InputLayout>, input_layout, { return inputLayout; })
	protected:
		ComPtr<ID3D11VertexShader> vertexShader;
		ComPtr<ID3D11PixelShader> pixelShader;
		ComPtr<ID3D11InputLayout> inputLayout;
	};

}