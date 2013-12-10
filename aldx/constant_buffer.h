#pragma once
#include "helper.h"

//constant_buffer
// A Shader constant_buffer. T is a struct/value that mirrors the shader's definition. constant_buffer keeps track of changes and only actually updates when needed
template <typename T>
class constant_buffer
{
	T _data;
	bool changed;
	int _slot;
	ComPtr<ID3D11Buffer> _buffer;
public:
	//get or set the data. getorset is true if you are setting and false if you are getting. defaults to setting(true)
	inline T& data(bool getorset = true) { changed = (!getorset ? changed : true); return _data; }
	inline int& bind_slot() { return _slot; }
	inline ComPtr<ID3D11Buffer>& buffer() { return _buffer; }

	constant_buffer() { }

	constant_buffer(ComPtr<ID3D11Device> device, int slot, T d = T())
		: _data(d), _slot(slot)
	{
		CD3D11_BUFFER_DESC bd(sizeof(T), D3D11_BIND_CONSTANT_BUFFER);
		chr(device->CreateBuffer(&bd, nullptr, _buffer.GetAddressOf()));
	}

	//Bind to a particular shader stage
	inline void bind(ComPtr<ID3D11DeviceContext> context, shader_stage stage)
	{
		switch (stage)
		{
		case shader_stage::Vertex:
			context->VSSetConstantBuffers(_slot, 1, _buffer.GetAddressOf());
			break;
		case shader_stage::Pixel:
			context->PSSetConstantBuffers(_slot, 1, _buffer.GetAddressOf());
			break;
		case shader_stage::Geometry:
			context->GSSetConstantBuffers(_slot, 1, _buffer.GetAddressOf());
			break;
		case shader_stage::Domain:
			context->DSSetConstantBuffers(_slot, 1, _buffer.GetAddressOf());
			break;
		case shader_stage::Hull:
			context->HSSetConstantBuffers(_slot, 1, _buffer.GetAddressOf());
			break;
		case shader_stage::Compute:
			context->CSGetConstantBuffers(_slot, 1, _buffer.GetAddressOf());
			break;
		}
	}	
	//Bind to a particular shader stage and slot
	inline void bind(ComPtr<ID3D11DeviceContext> context, shader_stage stage, int overrideSlot)
	{
		switch (stage)
		{
		case shader_stage::Vertex:
			context->VSSetConstantBuffers(overrideSlot, 1, _buffer.GetAddressOf());
			break;
		case shader_stage::Pixel:
			context->PSSetConstantBuffers(overrideSlot, 1, _buffer.GetAddressOf());
			break;
		case shader_stage::Geometry:
			context->GSSetConstantBuffers(overrideSlot, 1, _buffer.GetAddressOf());
			break;
		case shader_stage::Domain:
			context->DSSetConstantBuffers(overrideSlot, 1, _buffer.GetAddressOf());
			break;
		case shader_stage::Hull:
			context->HSSetConstantBuffers(overrideSlot, 1, _buffer.GetAddressOf());
			break;
		case shader_stage::Compute:
			context->CSSetConstantBuffers(overrideSlot, 1, _buffer.GetAddressOf());
			break;
		}
	}	

	//Unbind from a particular shader stage
	inline void unbind(ComPtr<ID3D11DeviceContext> context, shader_stage stage)
	{
		ID3D11Buffer* nullbuf[] = {nullptr};
		switch (stage)
		{
		case shader_stage::Vertex:
			context->VSSetConstantBuffers(_slot, 1, nullbuf);
			break;
		case shader_stage::Pixel:
			context->PSSetConstantBuffers(_slot, 1, nullbuf);
			break;
		case shader_stage::Geometry:
			context->GSSetConstantBuffers(_slot, 1, nullbuf);
			break;
		case shader_stage::Domain:
			context->DSSetConstantBuffers(_slot, 1, nullbuf);
			break;
		case shader_stage::Hull:
			context->HSSetConstantBuffers(_slot, 1, nullbuf);
			break;
		case shader_stage::Compute:
			context->CSSetConstantBuffers(_slot, 1, nullbuf);
			break;
		}
	}	
	//Unbind from a particular shader stage and slot
	inline void unbind(ComPtr<ID3D11DeviceContext> context, shader_stage stage, int overrideSlot)
	{
		ID3D11Buffer* nullbuf[] = {nullptr};
		switch (stage)
		{
		case shader_stage::Vertex:
			context->VSSetConstantBuffers(overrideSlot, 1, nullbuf);
			break;
		case shader_stage::Pixel:
			context->PSSetConstantBuffers(overrideSlot, 1, nullbuf);
			break;
		case shader_stage::Geometry:
			context->GSSetConstantBuffers(overrideSlot, 1, nullbuf);
			break;
		case shader_stage::Domain:
			context->DSSetConstantBuffers(overrideSlot, 1, nullbuf);
			break;
		case shader_stage::Hull:
			context->HSSetConstantBuffers(overrideSlot, 1, nullbuf);
			break;
		case shader_stage::Compute:
			context->CSSetConstantBuffers(overrideSlot, 1, nullbuf);
			break;
		}
	}	
	
	//Update the data on the GPU. Only happens if something has changed
	void update(ComPtr<ID3D11DeviceContext> context)
	{
		if(!changed) return;
		context->UpdateSubresource(_buffer.Get(), 0, nullptr, &_data, 0, 0);
	}
};
