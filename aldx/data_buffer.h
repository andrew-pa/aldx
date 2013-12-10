#pragma once
#include "helper.h"

//data_buffer<data_type>
// represents a shader StructuredBuffer<data_type>. works much like a texture.
// update will probably fail unless you create it with usage = D3D11_USAGE_DYNAMIC
template <typename data_type> class data_buffer
{
	ComPtr<ID3D11Buffer> _buffer;
	ComPtr<ID3D11ShaderResourceView> srv;
	vector<data_type> _data;
public:
	proprw(ComPtr<ID3D11Buffer>, buffer, { return _buffer; })
	proprw(ComPtr<ID3D11ShaderResourceView>, shader_view, { return srv; })
	proprw(vector<data_type>, data, { return _data; })

	data_buffer(){}

	data_buffer(ComPtr<ID3D11Device> device, vector<data_type> ddata,
		UINT misc_flags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED, 
		D3D11_USAGE usage = D3D11_USAGE_DEFAULT, bool cpu_access = false);
	data_buffer(ComPtr<ID3D11Device> device, data_type* ddata, size_t data_length,
		UINT misc_flags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
		D3D11_USAGE usage = D3D11_USAGE_DEFAULT, bool cpu_access = false);

	virtual void bind(ComPtr<ID3D11DeviceContext> context, shader_stage stage, int slot);
	virtual void unbind(ComPtr<ID3D11DeviceContext> context, shader_stage stage, int slot);
	virtual void update(ComPtr<ID3D11DeviceContext> context);
};

