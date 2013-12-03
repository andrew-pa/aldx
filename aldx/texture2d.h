#pragma once
#include "helper.h"

//texture2d
// Generic 2D texture. Provides both Texture2D and ShaderResourceView objects 
class texture2d
{
protected:
	ComPtr<ID3D11Texture2D> texd;
	ComPtr<ID3D11ShaderResourceView> srv;
public:
	inline const ComPtr<ID3D11Texture2D>& texture2D() const { return texd; }
	inline const ComPtr<ID3D11ShaderResourceView>& shader_resource_view() const { return srv; }

	texture2d() {}

	//Create Texture2D from a standing ShaderResourceView
	texture2d(ComPtr<ID3D11ShaderResourceView> _srv)
	{
		ComPtr<ID3D11Resource> res;
		_srv->GetResource(&res);
		res.As(&texd);
		srv = _srv;
	}

	//Create a empty Texture2D from a Texture description
	texture2d(ComPtr<ID3D11Device> device, CD3D11_TEXTURE2D_DESC desc)
	{
		chr(device->CreateTexture2D(&desc, nullptr, texd.GetAddressOf()));
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		srvd.Format = desc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = desc.MipLevels;
		srvd.Texture2D.MostDetailedMip = 0;
		chr(device->CreateShaderResourceView(texd.Get(), &srvd, srv.GetAddressOf()));
	}
	
	//Create a empty Texture2D with a Texture and SRV description
	texture2d(ComPtr<ID3D11Device> device, CD3D11_TEXTURE2D_DESC& desc,
		CD3D11_SHADER_RESOURCE_VIEW_DESC& srvdesc)
	{
		chr(device->CreateTexture2D(&desc, nullptr, texd.GetAddressOf()));
		chr(device->CreateShaderResourceView(texd.Get(), &srvdesc, srv.GetAddressOf()));
	}

	//Create a Texture2D from a DDS file
	texture2d(ComPtr<ID3D11Device> device, datablob<byte>* ddsData)
	{
		ComPtr<ID3D11Resource> res;
		//CreateDDSTextureFromMemory(device.Get(), ddsData->data, ddsData->length, res.GetAddressOf(), srv.GetAddressOf());
		throw exception("Implement this!");
		res.As(&texd);
	}

	virtual void texture2d::bind(ComPtr<ID3D11DeviceContext> context, ShaderStage stage, int slot = 0);
	virtual void texture2d::unbind(ComPtr<ID3D11DeviceContext> context, ShaderStage stage, int slot = 0);
};