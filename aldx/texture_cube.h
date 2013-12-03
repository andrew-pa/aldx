#pragma once
#include "helper.h"
#include "texture2d.h"

//texture_cube
// A Cubemap texture
class texture_cube : public texture2d
{
public:
	texture_cube(void) { }
	texture_cube(ComPtr<ID3D11Device> device, datablob<byte>* ddsData)
		: texture2d(device, ddsData) { }
	texture_cube(ComPtr<ID3D11Device> device, CD3D11_TEXTURE2D_DESC desc);
};
