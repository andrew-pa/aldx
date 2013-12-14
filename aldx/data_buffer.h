#pragma once
#include "helper.h"

namespace aldx
{
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
			D3D11_USAGE usage = D3D11_USAGE_DEFAULT, bool cpu_access = false) : _data(ddata)
		{
			D3D11_SUBRESOURCE_DATA sd = { 0 };
			sd.pSysMem = &ddata[0];
			CD3D11_BUFFER_DESC bd(sizeof(data_type)*ddata.size(), D3D11_BIND_SHADER_RESOURCE);
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
			srvd.BufferEx.NumElements = ddata.size();
			chr(device->CreateShaderResourceView(_buffer.Get(), &srvd, srv.GetAddressOf()));
		}
		data_buffer(ComPtr<ID3D11Device> device, data_type* ddata, size_t data_length,
			UINT misc_flags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
			D3D11_USAGE usage = D3D11_USAGE_DEFAULT, bool cpu_access = false) : _data(ddata)
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
			chr(device->CreateShaderResourceView(_buffer.Get(), &srvd, srv.GetAddressOf()));
		}

		void bind(ComPtr<ID3D11DeviceContext> context, shader_stage stage, int slot)
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
		void unbind(ComPtr<ID3D11DeviceContext> context, shader_stage stage, int slot)
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
		void update(ComPtr<ID3D11DeviceContext> context)
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			chr(context->Map(_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr));
			memcpy(msr.pData, &_data[0], _data.size()*sizeof(data_type));
			context->Unmap(_buffer.Get(), 0);
		}

	};
}
