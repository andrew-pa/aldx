
#include "texture_cube.h"

namespace aldx
{
	texture_cube::texture_cube(ComPtr<ID3D11Device> device,
		CD3D11_TEXTURE2D_DESC desc)
	{
		desc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
		chr(device->CreateTexture2D(&desc, nullptr, texd.GetAddressOf()));
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		srvd.Format = desc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvd.Texture2D.MipLevels = desc.MipLevels;
		srvd.Texture2D.MostDetailedMip = 0;
		chr(device->CreateShaderResourceView(texd.Get(), &srvd, srv.GetAddressOf()));
	}
}