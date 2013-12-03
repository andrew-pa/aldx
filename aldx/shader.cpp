
#include "shader.h"


shader::shader(void)
{
}

shader::shader(ComPtr<ID3D11Device> device, datablob<byte>* vs_shaderData, 
		datablob<byte>* ps_shaderData, const D3D11_INPUT_ELEMENT_DESC inputLayoutA[], int inputLayoutLength)
{
	if(vs_shaderData != nullptr)
		chr(device->CreateVertexShader(vs_shaderData->data, vs_shaderData->length, nullptr, &vertexShader));
	else
		vertexShader = nullptr;
	chr(device->CreateInputLayout(inputLayoutA, inputLayoutLength, vs_shaderData->data, vs_shaderData->length, &inputLayout));
	if(ps_shaderData != nullptr)
		chr(device->CreatePixelShader(ps_shaderData->data, ps_shaderData->length, nullptr, &pixelShader));
	else
		pixelShader = nullptr;
}

void shader::bind(ComPtr<ID3D11DeviceContext> context)
{
	context->IASetInputLayout(inputLayout.Get());
	context->VSSetShader(vertexShader.Get(), nullptr, 0);
	context->PSSetShader(pixelShader.Get(), nullptr, 0);
}

void shader::unbind(ComPtr<ID3D11DeviceContext> context)
{
	context->IASetInputLayout(nullptr);
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}