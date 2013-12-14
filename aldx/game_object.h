#pragma once
#include "helper.h"
#include "model.h"
#include "shader.h"
#include "render_shader.h"

namespace aldx
{
	//game_object
	// Simple object, has every thing that you need to render something. Not meant for advanced stuff, like shaders that don't use material, but instead use something else
	// Don't use this for a actual project! It's really only for demos/tests
	class game_object
	{
	public:
		struct game_object_material
		{
			material mat;
			uint texture_index;
			game_object_material()
				: mat(), texture_index(0) { }
			game_object_material(material m, uint t)
				: mat(m), texture_index(t) { }
		};
		game_object(void) : _model(nullptr), _world(), _shader(nullptr) { }
		game_object(model* m, render_shader* s, float4x4& w) : _model(m), _world(w), _shader(s), mats(m->meshes().size())
		{
		}

		game_object(model* m, render_shader* s, float3 _pos = float3(), float3 _rot = float3(), vector<texture2d*> texs = vector<texture2d*>(), float3 _scl = float3(1, 1, 1))
			: _model(m), _shader(s), mats(m->meshes().size()), pos(_pos), rot(_rot), scl(_scl), _textures(texs)
		{
		}

		//Update the world matrix of this object
		virtual void update(float t, float dt);

		//Draw the object
		virtual void draw(ComPtr<ID3D11DeviceContext1> context);
		//Draw the object with a diffrent shader
		virtual void draw_with_shader(ComPtr<ID3D11DeviceContext1> context, render_shader* shad);

		proprw(float3, position, { return pos; })
			proprw(float3, rotation, { return rot; })
			proprw(float3, scale, { return scl; });

		proprw(model*, Model, { return _model; })
			proprw(float4x4, world, { return _world; })
			inline void worldmx(const matrix& m) { store4x4(_world, m); }
		proprw(render_shader*, rnshader, { return _shader; })
			proprw(std::vector<texture2d*>, textures, { return _textures; })
			proprw(std::vector<game_object_material>, materials, { return mats; });
	protected:
		model* _model;
		float4x4 _world;
		render_shader* _shader;
		std::vector<texture2d*> _textures;
		std::vector<game_object_material> mats;

		float3 pos, rot, scl;
	};
}
