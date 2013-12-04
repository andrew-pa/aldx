
#include "game_object.h"

void game_object::update(float,float)
{
	load3(pos);
	load3(rot);
	load3(scl);
	worldmx(
		XMMatrixScalingFromVector(vscl) * 
		XMMatrixRotationRollPitchYawFromVector(vrot) * 
		XMMatrixTranslationFromVector(vpos));
}

void game_object::draw(ComPtr<ID3D11DeviceContext1> context)
{
	_shader->Bind(context);
	
	for(uint i = 0; i < _model->meshes().size(); ++i)
	{
		_shader->world(_world*(_model->worlds()[i]));
		_shader->set_material(mats[i].mat);
		_shader->set_texture(_textures[mats[i].texture_index]);
		_shader->update(context);
		_model->meshes()[i]->draw(context);
	}
}

void game_object::draw_with_shader(ComPtr<ID3D11DeviceContext1> context, render_shader* shad)
{
	shad->Bind(context);
	
	for(uint i = 0; i < _model->meshes().size(); ++i)
	{
		shad->world(_world*(_model->worlds()[i]));
		shad->set_material(mats[i].mat);
		shad->set_texture(_textures[mats[i].texture_index]);
		shad->update(context);
		_model->meshes()[i]->draw(context);
	}
}