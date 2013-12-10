#pragma once
#include "helper.h"

template <typename data_type>
class data_buffer
{
	ComPtr<ID3D11Buffer> _buffer;
	ComPtr<ID3D11ShaderResourceView> srv;
public:
	proprw(ComPtr<ID3D11Buffer>, buffer, { return _buffer; })
	proprw(ComPtr<ID3D11ShaderResourceView>, shader_view, { return srv; })

	data_buffer(ComPtr<ID3D11Device> device, vector<data_type> data,
		UINT misc_flags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED, 
		D3D11_USAGE usage = D3D11_USAGE_DEFAULT, bool cpu_access = false);
	data_buffer(ComPtr<ID3D11Device> device, data_type* data, size_t data_length,
		UINT misc_flags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
		D3D11_USAGE usage = D3D11_USAGE_DEFAULT, bool cpu_access = false);

	virtual void bind(ComPtr<ID3D11DeviceContext> context, shader_stage stage, int slot);
	virtual void unbind(ComPtr<ID3D11DeviceContext> context, shader_stage stage, int slot);
};

