#include "data_buffer.h"


template <typename data_type>
data_buffer<data_type>::data_buffer(ComPtr<ID3D11Device> device, vector<data_type> data,
	UINT misc_flags,
	D3D11_USAGE usage, bool cpu_access)
{
	D3D11_SUBRESOURCE_DATA sd = { 0 };
	sd.pSysMem = &data[0];
	CD3D11_BUFFER_DESC bd(sizeof(data_type)*data.size(), D3D11_BIND_SHADER_RESOURCE);
	bd.Usage = usage;
	bd.StructureByteStride = sizeof(data_type);
	bd.MiscFlags = misc_flags;
	bd.CPUAccessFlags = (cpu_access ? D3D11_CPU_ACCESS_WRITE : 0);
	chr(device->CreateBuffer(&bd, &sd, _buffer.GetAddressOf()));

	CD3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	srvd.Format = DXGI_FORMAT_UNKNOWN;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvd.BufferEx.FirstElement = 0;
	srvd.BufferEx.Flags = 0;
	srvd.BufferEx.NumElements = data.size();
	device->CreateShaderResourceView(_buffer.Get(), &srvd, srv.GetAddressOf());
}

template <typename data_type>
data_buffer<data_type>::data_buffer(ComPtr<ID3D11Device> device, data_type* data, size_t data_length,
	UINT misc_flags,
	D3D11_USAGE usage, bool cpu_access)
{
	D3D11_SUBRESOURCE_DATA sd = { 0 };
	sd.pSysMem = data;
	CD3D11_BUFFER_DESC bd(sizeof(data_type)*data_length, D3D11_BIND_SHADER_RESOURCE);
	bd.Usage = usage;
	bd.StructureByteStride = sizeof(data_type);
	bd.MiscFlags = misc_flags;
	bd.CPUAccessFlags = (cpu_access ? D3D11_CPU_ACCESS_WRITE : 0);
	chr(device->CreateBuffer(&bd, &sd, _buffer.GetAddressOf()));

	CD3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	srvd.Format = DXGI_FORMAT_UNKNOWN;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvd.BufferEx.FirstElement = 0;
	srvd.BufferEx.Flags = 0;
	srvd.BufferEx.NumElements = data_length;
	device->CreateShaderResourceView(_buffer.Get(), &srvd, srv.GetAddressOf());
}


template <typename data_type>
void data_buffer<data_type>::bind(ComPtr<ID3D11DeviceContext> context, shader_stage stage, int slot)
{
	switch (stage)
	{
	case shader_stage::Vertex:
		context->VSSetShaderResources(slot, 1, srv.GetAddressOf());
		break;
	case shader_stage::Pixel:
		context->PSSetShaderResources(slot, 1, srv.GetAddressOf());
		break;
	case shader_stage::Geometry:
		context->GSSetShaderResources(slot, 1, srv.GetAddressOf());
		break;
	case shader_stage::Domain:
		context->DSSetShaderResources(slot, 1, srv.GetAddressOf());
		break;
	case shader_stage::Hull:
		context->HSSetShaderResources(slot, 1, srv.GetAddressOf());
		break;
	case shader_stage::Compute:
		context->CSSetShaderResources(slot, 1, srv.GetAddressOf());
		break;
	}
}

template <typename data_type>
void data_buffer<data_type>::unbind(ComPtr<ID3D11DeviceContext> context, shader_stage stage, int slot)
{
	ID3D11ShaderResourceView* nullsrvs[] = { nullptr };
	switch (stage)
	{
	case shader_stage::Vertex:
		context->VSSetShaderResources(slot, 1, nullsrvs);
		break;
	case shader_stage::Pixel:
		context->PSSetShaderResources(slot, 1, nullsrvs);
		break;
	case shader_stage::Geometry:
		context->GSSetShaderResources(slot, 1, nullsrvs);
		break;
	case shader_stage::Domain:
		context->DSSetShaderResources(slot, 1, nullsrvs);
		break;
	case shader_stage::Hull:
		context->HSSetShaderResources(slot, 1, nullsrvs);
		break;
	case shader_stage::Compute:
		context->CSSetShaderResources(slot, 1, nullsrvs);
		break;
	}
}