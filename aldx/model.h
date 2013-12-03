#pragma once
#include "helper.h"
#include "mesh.h"
#include "bofile.h"

//system_mesh
// mesh in system memory
struct system_mesh
{
public:
	system_mesh(void* v, void* i, uint32 vx, uint32 ix, std::string _name = "")
		: verts(v), idxs(i), vc(vx), ic(ix), name(_name) {}
	std::string name;
	void* verts, *idxs;
	uint32 vc, ic;
};

//model
// A collection of meshes, with associated materials, textures, worlds
class model
{
public:
	model() { };
	model(mesh* mesh)
	{
		_meshes.push_back(mesh);
		float4x4 w;
		store4x4(w, XMMatrixIdentity());
		_worlds.push_back(w);
	}
	model(std::vector<mesh*> meshes, std::vector<float4x4> worlds);
	model(ComPtr<ID3D11Device> device, void* filedata);
	model(ComPtr<ID3D11Device> device, void* filedata,
		std::vector<system_mesh>& sysmeshs);
	model(ComPtr<ID3D11Device> device, bo_file* b);

	proprw(std::vector<mesh*>, Meshes, { return _meshes; })
	proprw(std::vector<float4x4>, Worlds, { return _worlds; })

	struct vertex
	{
		float3 pos;
		float3 norm;
		float2 texc;
		vertex()
		{}
		vertex(float px, float py, float pz, float nx, float ny, float nz, float3 col)
			: pos(px, py, pz), norm(nx, ny, nz), texc(0, 0)
		{
		}
		vertex(float px, float py, float pz, float nx, float ny, float nz, float tx = 0, float ty = 0, float tz = 0, float u = 0, float v = 0)
			:pos(px,py,pz), norm(nx, ny, nz), texc(u, v)
		{
		}
	};
protected:
	std::vector<mesh*> _meshes;
	std::vector<float4x4> _worlds;
};

//ModelLoadData
// Load the data from a file in to a vector of sysmeshes
// DECAP
void model_load_data(void* filedata, std::vector<system_mesh>& sysmeshs);

void model_load_data_from_bo(bo_file* b, vector<system_mesh>& sysmeshs);
