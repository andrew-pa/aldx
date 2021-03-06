
#include "model.h"

namespace aldx
{
	model::model(std::vector<mesh*> meshes, std::vector<float4x4> worlds)
	{
		_meshes = meshes;
		_worlds = worlds;
	}
	model::model(ComPtr<ID3D11Device> device, void* filedata)
	{
		uint8* data = (uint8*)filedata;
		uint32 meshcount = *((uint32*)filedata);
		size_t offset = sizeof(uint32);
		float4x4 id;
		XMStoreFloat4x4(&id, XMMatrixIdentity());
		for (uint32 c = 0; c < meshcount; c++)
		{
			uint32 numvertex = *reinterpret_cast<uint32*>(data + offset); offset += sizeof(uint32);
			uint32 numindex = *reinterpret_cast<uint32*>(data + offset); offset += sizeof(uint32);
			void* vert = reinterpret_cast<vertex*>(data + offset); offset += sizeof(vertex)*numvertex;
			void* indx = reinterpret_cast<uint32*>(data + offset); offset += sizeof(uint32)*numindex;
			_meshes.push_back(new mesh(device, vert, indx, numindex, numvertex, sizeof(vertex)));
			_worlds.push_back(id);
		}
	}
	model::model(ComPtr<ID3D11Device> device, void* filedata,
		std::vector<system_mesh>& sysmeshs)
	{
		sysmeshs.clear();
		uint8* data = (uint8*)filedata;
		uint32 meshcount = *((uint32*)filedata);
		size_t offset = sizeof(uint32);
		float4x4 id;
		XMStoreFloat4x4(&id, XMMatrixIdentity());
		for (uint32 c = 0; c < meshcount; c++)
		{
			uint32 numvertex = *reinterpret_cast<uint32*>(data + offset); offset += sizeof(uint32);
			uint32 numindex = *reinterpret_cast<uint32*>(data + offset); offset += sizeof(uint32);
			void* vert = reinterpret_cast<vertex*>(data + offset); offset += sizeof(vertex)*numvertex;
			void* indx = reinterpret_cast<uint32*>(data + offset); offset += sizeof(uint32)*numindex;
			_meshes.push_back(new mesh(device, vert, indx, numindex, numvertex, sizeof(vertex)));
			_worlds.push_back(id);
			sysmeshs.push_back(system_mesh(vert, indx, numvertex, numindex));
		}
	}
	void ModelLoadData(void* filedata, std::vector<system_mesh>& sysmeshs)
	{
		sysmeshs.clear();
		uint8* data = (uint8*)filedata;
		uint32 meshcount = *((uint32*)filedata);
		size_t offset = sizeof(uint32);
		float4x4 id;
		XMStoreFloat4x4(&id, XMMatrixIdentity());
		for (uint32 c = 0; c < meshcount; c++)
		{
			uint32 numvertex = *reinterpret_cast<uint32*>(data + offset); offset += sizeof(uint32);
			uint32 numindex = *reinterpret_cast<uint32*>(data + offset); offset += sizeof(uint32);
			void* vert = reinterpret_cast<model::vertex*>(data + offset); offset += sizeof(model::vertex)*numvertex;
			void* indx = reinterpret_cast<uint32*>(data + offset); offset += sizeof(uint32)*numindex;
			sysmeshs.push_back(system_mesh(vert, indx, numvertex, numindex));
		}
	}

	model::model(ComPtr<ID3D11Device> device, bo_file* b)
	{
		if (b->type != 8)
			throw exception("bo file doesn't contain a mesh");

		vector<mesh*> meshes;
		vector<float4x4> worlds;
		float4x4 id;
		XMStoreFloat4x4(&id, XMMatrixIdentity());
		for (auto o = b->data->begin(); o != b->data->end(); ++o)
		{
			byte* bd = (byte*)o->second.data;
			uint* d = (uint*)bd;
			uint vc = d[0];
			uint ic = d[1];
			void* v = (bd + 2 * sizeof(uint));
			void* i = (bd + 2 * sizeof(uint)+sizeof(dvertex)*vc);
			meshes.push_back(new mesh(device, v, i, ic, vc, sizeof(dvertex), o->first));
			worlds.push_back(id); //need to extract world from data also...
		}
		
		_meshes = meshes;
		_worlds = worlds;
	}

	void ModelLoadDataFromBO(bo_file* b, vector<system_mesh>& sysmeshs)
	{
		sysmeshs.clear();
		if (b->type != 8)
			return;

		vector<mesh*> meshes;
		vector<float4x4> worlds;
		float4x4 id;
		XMStoreFloat4x4(&id, XMMatrixIdentity());
		for (auto o = b->data->begin(); o != b->data->end(); ++o)
		{
			byte* bd = (byte*)o->second.data;
			uint* d = (uint*)bd;
			uint vc = d[0];
			uint ic = d[1];
			void* v = (bd + 2 * sizeof(uint));
			void* i = (bd + 2 * sizeof(uint)+sizeof(dvertex)*vc);
			sysmeshs.push_back(system_mesh(v, i, vc, ic, o->first));
		}
	}
}