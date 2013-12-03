
#include "game_object.h"

voidgame_object::Update(float,float)
{
	load3(pos);
	load3(rot);
	load3(scl);
	worldmx(
		XMMatrixScalingFromVector(vscl) * 
		XMMatrixRotationRollPitchYawFromVector(vrot) * 
		XMMatrixTranslationFromVector(vpos));
}

voidgame_object::Draw(ComPtr<ID3D11DeviceContext1> context)
{
	_shader->Bind(context);
	
	for(uint i = 0; i < _model->Meshes().size(); ++i)
	{
		_shader->world(_world*(_model->Worlds()[i]));
		_shader->setMaterial(mats[i].mat);
		_shader->setTexture(_textures[mats[i].texture_index]);
		_shader->Update(context);
		_model->Meshes()[i]->Draw(context);
	}
}

voidgame_object::DrawWithShader(ComPtr<ID3D11DeviceContext1> context, IRenderShader* shad)
{
	shad->Bind(context);
	
	for(uint i = 0; i < _model->Meshes().size(); ++i)
	{
		shad->world(_world*(_model->Worlds()[i]));
		shad->setMaterial(mats[i].mat);
		shad->setTexture(_textures[mats[i].texture_index]);
		shad->Update(context);
		_model->Meshes()[i]->Draw(context);
	}
}