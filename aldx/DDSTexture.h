//--------------------------------------------------------------------------------------
// File: DDSTextureLoader.h
//
// Function for loading a DDS texture and creating a Direct3D 11 runtime resource for it
//
// Note this function is useful as a light-weight runtime loader for DDS files. For
// a full-featured DDS file reader, writer, and texture processing pipeline see
// the 'Texconv' sample and the 'DirectXTex' library.
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma once
#include "helper.h"

//void CreateDDSTextureFromMemory(
//	_In_ ID3D11Device* d3dDevice,
//	_In_reads_bytes_(ddsDataSize) const byte* ddsData,
//	_In_ size_t ddsDataSize,
//	_Out_opt_ ID3D11Resource** texture,
//	_Out_opt_ ID3D11ShaderResourceView** textureView,
//	_In_ size_t maxsize = 0
//	);
//
//inline ID3D11ShaderResourceView* CreateSRVDDSTextureFromMemory(ID3D11Device* d, const byte* ddsData, size_t ddsDataSize)
//{
//	ID3D11ShaderResourceView* r;
//	CreateDDSTextureFromMemory(d, ddsData, ddsDataSize, nullptr, &r);
//	return r;
//}
//
////DDSCreateSRVFromMemory : Wrapper around CreateSRVDDSTextureFromMemory for a datablob<byte>
//inline ID3D11ShaderResourceView* DDSCreateSRVFromMemory(ID3D11Device* device, datablob<byte>* data)
//{
//	return CreateSRVDDSTextureFromMemory(device, data->data, data->length);
//}